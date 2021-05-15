#include "executer.h"

void Executer::execute(std::vector<Lexeme> &poliz) {
    Lexeme pc_el;
    std::stack<Lexeme> args;
    int i, index = 0, size = poliz.size(), j;
    Lexeme one, two;
    std::stack<int> inc_st;
    std::stack<int> dec_st;
    int inc_dec_tmp;

    while (index < size) {
        pc_el = poliz[index];
        switch (pc_el.get_type()) {
            case LEX_TRUE:
            case LEX_FALSE:
            case LEX_NUM:
            case POLIZ_ADDRESS:
            case POLIZ_LABEL:;
            case LEX_STR_CONST: {
                args.push(pc_el);
                break;
            }
            case LEX_ID: {
                i = pc_el.get_value();
                if (!TID[i].get_assign()) {
                    throw "Identifier Error";
                }
                args.push(Lexeme(TID[i].get_type(), TID[i].get_value(), TID[i].get_str()));
                break;
            }
            case POLIZ_GO: {
                from_st(args, one);
                index = one.get_value() - 1;
                break;
            }
            case POLIZ_FGO: {
                from_st(args, one);
                from_st(args, two);
                if (two.get_type() != LEX_TRUE && two.get_type() != LEX_FALSE) {
                    throw "FGO error";
                }
                if (two.get_type() == LEX_FALSE) {
                    index = one.get_value() - 1;
                }
                break;
            }
            case LEX_PLUS: {
                from_st(args, one);
                from_st(args, two);
                if (one.get_type() == LEX_TRUE) {
                    one.put_value(1);
                    one.put_type(LEX_NUM);
                } else if (one.get_type() == LEX_FALSE) {
                    one.put_value(0);
                    one.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_TRUE) {
                    two.put_value(1);
                    two.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_FALSE) {
                    two.put_value(0);
                    two.put_type(LEX_NUM);
                }
                if (one.get_type() == LEX_STR_CONST &&
                    two.get_type() == LEX_STR_CONST) {
                    std::string str = two.get_string() + one.get_string();
                    one.put_string(str);
                } else if (one.get_type() == LEX_NUM && two.get_type() == LEX_NUM) {
                    j = one.get_value() + two.get_value();
                    one.put_value(j);
                } else if (one.get_type() == LEX_NUM &&
                           two.get_type() == LEX_STR_CONST) {
                    std::string str = two.get_string() + std::to_string(one.get_value());
                    one.put_type(LEX_STR_CONST);
                    one.put_string(str);
                } else if (one.get_type() == LEX_STR_CONST &&
                           two.get_type() == LEX_NUM) {
                    std::string str = std::to_string(two.get_value()) + one.get_string();
                    one.put_string(str);
                } else {
                    throw "PLUS error";
                }
                args.push(one);
                break;
            }
            case LEX_PLUS_EQ: {
                from_st(args, two);
                from_st(args, one);
                j = one.get_value();
                if (TID[j].get_assign()) {
                    std::cout << one.get_type() << " " << two.get_type() << std::endl;
                    if (TID[j].get_type() == LEX_TRUE) {
                        TID[j].put_value(1);
                        TID[j].put_type(LEX_NUM);
                    } else if (TID[j].get_type() == LEX_FALSE) {
                        TID[j].put_value(0);
                        TID[j].put_type(LEX_NUM);
                    } else if (two.get_type() == LEX_TRUE) {
                        two.put_value(1);
                        two.put_type(LEX_NUM);
                    } else if (two.get_type() == LEX_FALSE) {
                        two.put_value(0);
                        two.put_type(LEX_NUM);
                    }
                    if (TID[j].get_type() == LEX_STR_CONST &&
                        two.get_type() == LEX_STR_CONST) {
                        TID[j].put_str(TID[j].get_str() + two.get_string());
                    } else if (one.get_type() == POLIZ_ADDRESS &&
                               two.get_type() == LEX_NUM) {
                        TID[j].put_value(TID[j].get_value() + two.get_value());
                    } else {
                        throw "PLUS EQ error 1";
                    }
                } else {
                    throw "PLUS EQ error 2";
                }

                while (!inc_st.empty()) {
                    inc_dec_tmp = inc_st.top();
                    inc_st.pop();
                    TID[inc_dec_tmp].put_value(TID[inc_dec_tmp].get_value() + 1);
                }
                while (!dec_st.empty()) {
                    inc_dec_tmp = dec_st.top();
                    dec_st.pop();
                    TID[inc_dec_tmp].put_value(TID[inc_dec_tmp].get_value() - 1);
                }
                break;
            }
            case LEX_TIMES: {
                from_st(args, one);
                from_st(args, two);
                if (one.get_type() == LEX_TRUE) {
                    one.put_value(1);
                    one.put_type(LEX_NUM);
                } else if (one.get_type() == LEX_FALSE) {
                    one.put_value(0);
                    one.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_TRUE) {
                    two.put_value(1);
                    two.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_FALSE) {
                    two.put_value(0);
                    two.put_type(LEX_NUM);
                }
                if (one.get_type() == LEX_NUM && two.get_type() == LEX_NUM) {
                    int j = one.get_value() * two.get_value();
                    one.put_value(j);
                } else {
                    throw "TIMES error";
                }
                args.push(one);
                break;
            }
            case LEX_TIMES_EQ: {
                from_st(args, two);
                from_st(args, one);
                j = one.get_value();
                if (TID[j].get_assign()) {
                    if (TID[j].get_type() == LEX_TRUE) {
                        TID[j].put_value(1);
                        TID[j].put_type(LEX_NUM);
                    } else if (TID[j].get_type() == LEX_FALSE) {
                        TID[j].put_value(0);
                        TID[j].put_type(LEX_NUM);
                    } else if (two.get_type() == LEX_TRUE) {
                        two.put_value(1);
                        two.put_type(LEX_NUM);
                    } else if (two.get_type() == LEX_FALSE) {
                        two.put_value(0);
                        two.put_type(LEX_NUM);
                    } else if (one.get_type() == LEX_NUM && two.get_type() == LEX_NUM) {
                        TID[j].put_value(TID[j].get_value() * two.get_value());
                    } else {
                        throw "TIMES EQ error";
                    }
                } else {
                    throw "TIMES EQ error";
                }
                while (!inc_st.empty()) {
                    inc_dec_tmp = inc_st.top();
                    inc_st.pop();
                    TID[inc_dec_tmp].put_value(TID[inc_dec_tmp].get_value() + 1);
                }
                while (!dec_st.empty()) {
                    inc_dec_tmp = dec_st.top();
                    dec_st.pop();
                    TID[inc_dec_tmp].put_value(TID[inc_dec_tmp].get_value() - 1);
                }
                break;
            }
            case LEX_MINUS: {
                from_st(args, one);
                from_st(args, two);
                if (one.get_type() == LEX_TRUE) {
                    one.put_value(1);
                    one.put_type(LEX_NUM);
                } else if (one.get_type() == LEX_FALSE) {
                    one.put_value(0);
                    one.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_TRUE) {
                    two.put_value(1);
                    two.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_FALSE) {
                    two.put_value(0);
                    two.put_type(LEX_NUM);
                }
                if (one.get_type() == LEX_NUM && two.get_type() == LEX_NUM) {
                    int j = two.get_value() - one.get_value();
                    one.put_value(j);
                } else {
                    throw "MINUS error";
                }
                args.push(one);
                break;
            }
            case LEX_MINUS_EQ: {
                from_st(args, two);
                from_st(args, one);
                j = one.get_value();
                if (TID[j].get_assign()) {
                    if (TID[j].get_type() == LEX_TRUE) {
                        TID[j].put_value(1);
                        TID[j].put_type(LEX_NUM);
                    } else if (TID[j].get_type() == LEX_FALSE) {
                        TID[j].put_value(0);
                        TID[j].put_type(LEX_NUM);
                    } else if (two.get_type() == LEX_TRUE) {
                        two.put_value(1);
                        two.put_type(LEX_NUM);
                    } else if (two.get_type() == LEX_FALSE) {
                        two.put_value(0);
                        two.put_type(LEX_NUM);
                    } else if (one.get_type() == POLIZ_ADDRESS &&
                               two.get_type() == LEX_NUM) {
                        TID[j].put_value(TID[j].get_value() - two.get_value());
                    } else {
                        throw "MINUS EQ error";
                    }
                } else {
                    throw "MINUS EQ error";
                }
                while (!inc_st.empty()) {
                    inc_dec_tmp = inc_st.top();
                    inc_st.pop();
                    TID[inc_dec_tmp].put_value(TID[inc_dec_tmp].get_value() + 1);
                }
                while (!dec_st.empty()) {
                    inc_dec_tmp = dec_st.top();
                    dec_st.pop();
                    TID[inc_dec_tmp].put_value(TID[inc_dec_tmp].get_value() - 1);
                }
                break;
            }
            case LEX_SLASH: {
                from_st(args, two);
                from_st(args, one);
                if (one.get_type() == LEX_TRUE) {
                    one.put_value(1);
                    one.put_type(LEX_NUM);
                } else if (one.get_type() == LEX_FALSE) {
                    one.put_value(0);
                    one.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_TRUE) {
                    two.put_value(1);
                    two.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_FALSE) {
                    two.put_value(0);
                    two.put_type(LEX_NUM);
                }
                if (one.get_type() == LEX_NUM && two.get_type() == LEX_NUM) {
                    if (two.get_type()) {
                        int j = one.get_value() / two.get_value();
                        one.put_value(j);
                    } else {
                        throw "Divide by zero";
                    }
                } else {
                    throw "SLASH error";
                }
                args.push(one);
                break;
            }
            case LEX_SLASH_EQ:
                from_st(args, two);
                from_st(args, one);
                j = one.get_value();
                if (TID[j].get_assign()) {
                    if (TID[j].get_type() == LEX_TRUE) {
                        TID[j].put_value(1);
                        TID[j].put_type(LEX_NUM);
                    } else if (TID[j].get_type() == LEX_FALSE) {
                        TID[j].put_value(0);
                        TID[j].put_type(LEX_NUM);
                    } else if (two.get_type() == LEX_TRUE) {
                        two.put_value(1);
                        two.put_type(LEX_NUM);
                    } else if (two.get_type() == LEX_FALSE) {
                        two.put_value(0);
                        two.put_type(LEX_NUM);
                    } else if (one.get_type() == LEX_NUM && two.get_type() == LEX_NUM) {
                        if (!one.get_value()) {
                            TID[j].put_value(TID[j].get_value() / two.get_value());
                        } else {
                            throw "Divide by zero";
                        }
                    } else {
                        throw "SLASH EQ error";
                    }
                } else {
                    throw "SLASH EQ error";
                }
                while (!inc_st.empty()) {
                    inc_dec_tmp = inc_st.top();
                    inc_st.pop();
                    TID[inc_dec_tmp].put_value(TID[inc_dec_tmp].get_value() + 1);
                }
                while (!dec_st.empty()) {
                    inc_dec_tmp = dec_st.top();
                    dec_st.pop();
                    TID[inc_dec_tmp].put_value(TID[inc_dec_tmp].get_value() - 1);
                }
                break;

            case LEX_PERCENT: {
                from_st(args, two);
                from_st(args, one);
                if (one.get_type() == LEX_TRUE) {
                    one.put_value(1);
                    one.put_type(LEX_NUM);
                } else if (one.get_type() == LEX_FALSE) {
                    one.put_value(0);
                    one.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_TRUE) {
                    two.put_value(1);
                    two.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_FALSE) {
                    two.put_value(0);
                    two.put_type(LEX_NUM);
                }
                if (one.get_type() == LEX_NUM && two.get_type() == LEX_NUM) {
                    if (!two.get_type()) {
                        int j = one.get_value() % two.get_value();
                        one.put_value(j);
                    } else {
                        throw "Divide by zero";
                    }
                } else {
                    throw "Percent error";
                }
                args.push(one);
                break;
            }
            case LEX_PERCENT_EQ: {
                from_st(args, two);
                from_st(args, one);
                j = one.get_value();
                if (TID[j].get_assign()) {
                    if (TID[j].get_type() == LEX_TRUE) {
                        TID[j].put_value(1);
                        TID[j].put_type(LEX_NUM);
                    } else if (TID[j].get_type() == LEX_FALSE) {
                        TID[j].put_value(0);
                        TID[j].put_type(LEX_NUM);
                    } else if (two.get_type() == LEX_TRUE) {
                        two.put_value(1);
                        two.put_type(LEX_NUM);
                    } else if (two.get_type() == LEX_FALSE) {
                        two.put_value(0);
                        two.put_type(LEX_NUM);
                    } else if (one.get_type() == LEX_NUM && two.get_type() == LEX_NUM) {
                        if (!one.get_value()) {
                            TID[j].put_value(TID[j].get_value() % two.get_value());
                        } else {
                            throw "Divide by zero";
                        }
                    } else {
                        throw "Percent EQ error";
                    }
                } else {
                    throw "Percent EQ error";
                }
                while (!inc_st.empty()) {
                    inc_dec_tmp = inc_st.top();
                    inc_st.pop();
                    TID[inc_dec_tmp].put_value(TID[inc_dec_tmp].get_value() + 1);
                }
                while (!dec_st.empty()) {
                    inc_dec_tmp = dec_st.top();
                    dec_st.pop();
                    TID[inc_dec_tmp].put_value(TID[inc_dec_tmp].get_value() - 1);
                }
                break;
            }
            case LEX_EQ: {
                from_st(args, one);
                from_st(args, two);
                j = two.get_value();
                if (two.get_type() == LEX_NUM || two.get_type() == LEX_STR_CONST) {
                    args.push(one);
                }
                if (one.get_type() == LEX_STR_CONST) {
                    TID[j].put_str(one.get_string());
                    TID[j].put_type(LEX_STR_CONST);
                } else if (one.get_type() == LEX_NUM) {
                    TID[j].put_value(one.get_value());
                    TID[j].put_type(LEX_NUM);
                } else if (one.get_type() == LEX_TRUE) {
                    TID[j].put_type(LEX_TRUE);
                } else if (one.get_type() == LEX_FALSE) {
                    TID[j].put_type(LEX_FALSE);
                }
                TID[j].put_assign();
                while (!inc_st.empty()) {
                    inc_dec_tmp = inc_st.top();
                    inc_st.pop();
                    TID[inc_dec_tmp].put_value(TID[inc_dec_tmp].get_value() + 1);
                }
                while (!dec_st.empty()) {
                    inc_dec_tmp = dec_st.top();
                    dec_st.pop();
                    TID[inc_dec_tmp].put_value(TID[inc_dec_tmp].get_value() - 1);
                }
                break;
            }
            case POLIZ_LEFT_INC: {
                from_st(args, one);
                if (one.get_type() != POLIZ_ADDRESS) {
                    throw "LEFT INK error";
                }
                j = one.get_value();
                if (TID[j].get_assign()) {
                    if (TID[j].get_type() == LEX_NUM) {
                        int k = TID[j].get_value();
                        TID[j].put_value(k + 1);
                        args.push(Lexeme(LEX_NUM, k + 1, pc_el.get_string()));
                    }
                } else {
                    throw "LEFT INK error";
                }
                break;
            }
            case POLIZ_RIGHT_INC: {
                from_st(args, one);
                if (one.get_type() != POLIZ_ADDRESS) {
                    throw "RIGHT INK error";
                }
                j = one.get_value();
                if (TID[j].get_assign()) {
                    if (TID[j].get_type() == LEX_NUM) {
                        int k;
                        args.push(
                            Lexeme(LEX_NUM, k = TID[j].get_value(), pc_el.get_string()));
                        inc_st.push(j);
                        TID[j].put_value(k + 1);
                    }
                } else {
                    throw "RIGHT INK error";
                }
                break;
            }
            case POLIZ_LEFT_DEC: {
                from_st(args, one);
                if (one.get_type() != POLIZ_ADDRESS) {
                    throw "LEFT DEC error";
                }
                j = one.get_value();
                if (TID[j].get_assign()) {
                    if (TID[j].get_type() == LEX_NUM) {
                        int k = TID[j].get_value();
                        TID[j].put_value(k - 1);
                        args.push(Lexeme(LEX_NUM, k - 1, pc_el.get_string()));
                    }
                } else {
                    throw "LEFT DEC error";
                }
                break;
            }
            case POLIZ_RIGHT_DEC: {
                from_st(args, one);
                if (one.get_type() != POLIZ_ADDRESS) {
                    throw "RIGHT DEC error";
                }
                j = one.get_value();
                if (TID[j].get_assign()) {
                    if (TID[j].get_type() == LEX_NUM) {
                        int k;
                        args.push(
                            Lexeme(LEX_NUM, k = TID[j].get_value(), pc_el.get_string()));
                        dec_st.push(j);
                        TID[j].put_value(k - 1);
                    }
                } else {
                    throw "RIGHT DEC error";
                }
                break;
            }
            case LEX_NOT: {
                from_st(args, one);
                if (one.get_type() == LEX_STR_CONST) {
                    throw "NOT Error";
                } else if (one.get_type() == LEX_NUM) {
                    int a = one.get_value();
                    if (a == 0) {
                        one.put_type(LEX_TRUE);
                    } else {
                        one.put_type(LEX_FALSE);
                    }
                } else if (one.get_type() == POLIZ_ADDRESS ||
                           one.get_type() == POLIZ_LABEL) {
                    throw "NOT Error";
                } else {
                    if (one.get_type() == LEX_TRUE) {
                        one.put_type(LEX_FALSE);
                    }
                    if (one.get_type() == LEX_FALSE) {
                        one.put_type(LEX_TRUE);
                    }
                }
                args.push(one);
                break;
            }
            case LEX_DPIPE: {
                from_st(args, one);
                from_st(args, two);
                if (one.get_type() == LEX_NUM) {
                    if (one.get_value() > 0) {
                        j = 1;
                    } else {
                        j = 0;
                    }
                } else if (one.get_type() == LEX_TRUE) {
                    j = 1;
                } else if (one.get_type() == LEX_FALSE) {
                    j = 0;
                } else {
                    throw "OR Error";
                }
                if (two.get_type() == LEX_NUM) {
                    if (two.get_value() > 0) {
                        j++;
                    }
                } else if (two.get_type() == LEX_TRUE) {
                    j++;
                } else if (two.get_type() == LEX_FALSE) {
                    // TODO
                } else {
                    throw "OR Error";
                }
                if (j == 0) {
                    one.put_type(LEX_FALSE);
                } else {
                    one.put_type(LEX_TRUE);
                }
                args.push(one);
                break;
            }
            case LEX_DAMP: {
                from_st(args, one);
                from_st(args, two);
                if (one.get_type() == LEX_NUM) {
                    if (one.get_value() > 0) {
                        j = 1;
                    } else {
                        j = 0;
                    }
                } else if (one.get_type() == LEX_TRUE) {
                    j = 1;
                } else if (one.get_type() == LEX_FALSE) {
                    j = 0;
                } else {
                    throw "AND Error";
                }
                if (two.get_type() == LEX_NUM) {
                    if (two.get_value() == 0) {
                        j = 0;
                    }
                } else if (two.get_type() == LEX_TRUE) {
                    // TODO
                } else if (two.get_type() == LEX_TRUE) {
                    j = 0;
                } else {
                    throw "AND Error";
                }
                if (j == 0) {
                    one.put_type(LEX_FALSE);
                } else {
                    one.put_type(LEX_TRUE);
                }
                args.push(one);
                break;
            }
            case LEX_DEQ: {
                from_st(args, two);
                from_st(args, one);
                if (one.get_type() == LEX_TRUE) {
                    one.put_value(1);
                    one.put_type(LEX_NUM);
                } else if (one.get_type() == LEX_FALSE) {
                    one.put_value(0);
                    one.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_TRUE) {
                    two.put_value(1);
                    two.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_FALSE) {
                    two.put_value(0);
                    two.put_type(LEX_NUM);
                } else {
                    if (one.get_type() == LEX_STR_CONST &&
                        two.get_type() == LEX_STR_CONST) {
                        if (one.get_string() == two.get_string()) {
                            one.put_type(LEX_TRUE);
                        } else {
                            one.put_type(LEX_FALSE);
                        }
                    } else if (one.get_type() == LEX_NUM && two.get_type() == LEX_NUM) {
                        if (one.get_value() == two.get_value()) {
                            one.put_type(LEX_TRUE);
                        } else {
                            one.put_type(LEX_FALSE);
                        }
                    } else {
                        throw "EQ error";
                    }
                }
                args.push(one);
                break;
            }
            case LEX_LSS: {
                from_st(args, two);
                from_st(args, one);
                if (one.get_type() == LEX_TRUE) {
                    one.put_value(1);
                    one.put_type(LEX_NUM);
                } else if (one.get_type() == LEX_FALSE) {
                    one.put_value(0);
                    one.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_TRUE) {
                    two.put_value(1);
                    two.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_FALSE) {
                    two.put_value(0);
                    two.put_type(LEX_NUM);
                }
                if (one.get_type() == LEX_STR_CONST &&
                    two.get_type() == LEX_STR_CONST) {
                    if (one.get_string() < two.get_string()) {
                        one.put_type(LEX_TRUE);
                    } else {
                        one.put_type(LEX_FALSE);
                    }
                } else if (one.get_type() == LEX_NUM && two.get_type() == LEX_NUM) {
                    if (one.get_value() < two.get_value()) {
                        one.put_type(LEX_TRUE);
                    } else {
                        one.put_type(LEX_FALSE);
                    }
                } else {
                    throw "LESS error";
                }
                args.push(one);
                break;
            }
            case LEX_GTR: {
                from_st(args, two);
                from_st(args, one);
                if (one.get_type() == LEX_TRUE) {
                    one.put_value(1);
                    one.put_type(LEX_NUM);
                } else if (one.get_type() == LEX_FALSE) {
                    one.put_value(0);
                    one.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_TRUE) {
                    two.put_value(1);
                    two.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_FALSE) {
                    two.put_value(0);
                    two.put_type(LEX_NUM);
                }
                if (one.get_type() == LEX_STR_CONST &&
                    two.get_type() == LEX_STR_CONST) {
                    if (one.get_string() > two.get_string()) {
                        one.put_type(LEX_TRUE);
                    } else {
                        one.put_type(LEX_FALSE);
                    }
                } else if (one.get_type() == LEX_NUM && two.get_type() == LEX_NUM) {
                    if (one.get_value() > two.get_value()) {
                        one.put_type(LEX_TRUE);
                    } else {
                        one.put_type(LEX_FALSE);
                    }
                } else {
                    throw "GREATER error";
                }
                args.push(one);
                break;
            }
            case LEX_LEQ: {
                from_st(args, two);
                from_st(args, one);
                if (one.get_type() == LEX_TRUE) {
                    one.put_value(1);
                    one.put_type(LEX_NUM);
                } else if (one.get_type() == LEX_FALSE) {
                    one.put_value(0);
                    one.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_TRUE) {
                    two.put_value(1);
                    two.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_FALSE) {
                    two.put_value(0);
                    two.put_type(LEX_NUM);
                }
                if (one.get_type() == LEX_STR_CONST &&
                    two.get_type() == LEX_STR_CONST) {
                    if (one.get_string() <= two.get_string()) {
                        one.put_type(LEX_TRUE);
                    } else {
                        one.put_type(LEX_FALSE);
                    }
                } else if (one.get_type() == LEX_NUM && two.get_type() == LEX_NUM) {
                    if (one.get_value() <= two.get_value()) {
                        one.put_type(LEX_TRUE);
                    } else {
                        one.put_type(LEX_FALSE);
                    }
                } else {
                    throw "LESS EQ error";
                }
                args.push(one);
                break;
            }
            case LEX_GEQ: {
                from_st(args, two);
                from_st(args, one);
                if (one.get_type() == LEX_TRUE) {
                    one.put_value(1);
                    one.put_type(LEX_NUM);
                } else if (one.get_type() == LEX_FALSE) {
                    one.put_value(0);
                    one.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_TRUE) {
                    two.put_value(1);
                    two.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_FALSE) {
                    two.put_value(0);
                    two.put_type(LEX_NUM);
                }
                if (one.get_type() == LEX_STR_CONST &&
                    two.get_type() == LEX_STR_CONST) {
                    if (one.get_string() >= two.get_string()) {
                        one.put_type(LEX_TRUE);
                    } else {
                        one.put_type(LEX_FALSE);
                    }
                } else if (one.get_type() == LEX_NUM && two.get_type() == LEX_NUM) {
                    if (one.get_value() >= two.get_value()) {
                        one.put_type(LEX_TRUE);
                    } else {
                        one.put_type(LEX_FALSE);
                    }
                } else {
                    throw "GREATER EQ error";
                }
                args.push(one);
                break;
            }
            case LEX_NEQ: {
                from_st(args, two);
                from_st(args, one);
                if (one.get_type() == LEX_TRUE) {
                    one.put_value(1);
                    one.put_type(LEX_NUM);
                } else if (one.get_type() == LEX_FALSE) {
                    one.put_value(0);
                    one.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_TRUE) {
                    two.put_value(1);
                    two.put_type(LEX_NUM);
                } else if (two.get_type() == LEX_FALSE) {
                    two.put_value(0);
                    two.put_type(LEX_NUM);
                }
                if (one.get_type() == LEX_STR_CONST &&
                    two.get_type() == LEX_STR_CONST) {
                    if (one.get_string() != two.get_string()) {
                        one.put_type(LEX_TRUE);
                    } else {
                        one.put_type(LEX_FALSE);
                    }
                } else if (one.get_type() == LEX_NUM && two.get_type() == LEX_NUM) {
                    if (one.get_value() != two.get_value()) {
                        one.put_type(LEX_TRUE);
                    } else {
                        one.put_type(LEX_FALSE);
                    }
                } else {
                    throw "NOT EQ error";
                }
                args.push(one);
                break;
            }
            case POLIZ_WRITE: {
                from_st(args, one);
                if (one.get_type() == POLIZ_ADDRESS) {
                    if (!(TID[one.get_value()].get_str().empty())) {
                        std::cout << TID[one.get_value()].get_str() << std::endl;
                    } else {
                        std::cout << TID[one.get_value()].get_value() << std::endl;
                    }
                } else if (one.get_type() == LEX_STR_CONST) {
                    std::cout << one.get_string() << std::endl;
                } else {
                    std::cout << one.get_value() << std::endl;
                }
                break;
            }
            case POLIZ_GETENV: {
                from_st(args, one);
                std::cout << getenv(one.get_string().c_str()) << std::endl;
                break;
            }
            default: { throw "POLIZ: unexpected elem"; }
        }  // end of switch
        ++index;
    };  // end of while
    std::cout << CYAN_COLOR << "\n\nExecuter is OK!\n" << RESET_COLOR;
}