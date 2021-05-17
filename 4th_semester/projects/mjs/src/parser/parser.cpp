#include "parser.h"

void Parser::analyze() {
    int num = 0;
    std::cerr << CYAN_COLOR << "\nLEXEMS:\n\n"
              << RESET_COLOR;
    gl();
    S();
    if (c_type != LEX_FIN) {
        throw curr_lex;
    }
    std::cerr << CYAN_COLOR << "\nPOLIZ:\n\n"
              << RESET_COLOR;
    for (Lexeme l : poliz) {
        std::cerr << num << ' ' << l;
        num++;
    }
    std::cerr << CYAN_COLOR << "\nPOLIZ is OK!\n\n"
              << RESET_COLOR;
}

void Parser::S() {
    int pl0, pl1, pl2, pl3;
    if (c_type == LEX_IF) {
        gl();
        if (c_type != LEX_LPAREN) {
            throw curr_lex;
        } else {
            gl();
            E();
            pl2 = poliz.size();
            poliz.push_back(Lexeme());
            poliz.push_back(Lexeme(POLIZ_FGO));
            if (c_type == LEX_RPAREN) {
                gl();
                B();
                pl3 = poliz.size();
                poliz.push_back(Lexeme());
                poliz.push_back(Lexeme(POLIZ_GO));
                if (c_type == LEX_ELSE) {
                    poliz[pl2] = Lexeme(POLIZ_LABEL, poliz.size());
                    gl();
                    B();
                    poliz[pl3] = Lexeme(POLIZ_LABEL, poliz.size());
                } else {
                    poliz.pop_back();
                    poliz.pop_back();
                    poliz[pl2] = Lexeme(POLIZ_LABEL, poliz.size());
                }
            } else {
                throw curr_lex;
            }
        }
    } else if (c_type == LEX_WHILE) {
        pl0 = poliz.size();
        labels_for_con.push(poliz.size());
        gl();
        if (c_type != LEX_LPAREN) {
            throw curr_lex;
        } else {
            gl();
            E();
            pl1 = poliz.size();
            poliz.push_back(Lexeme());
            poliz.push_back(Lexeme(POLIZ_FGO));
            if (c_type == LEX_RPAREN) {
                gl();
                B();
                poliz.push_back(Lexeme(POLIZ_LABEL, pl0));
                poliz.push_back(Lexeme(POLIZ_GO));
                poliz[pl1] = Lexeme(POLIZ_LABEL, poliz.size());
            } else
                throw curr_lex;
        }
        if (!labels_for_break.empty()) {
            poliz[labels_for_break.top()] = Lexeme(POLIZ_LABEL, poliz.size());
            labels_for_break.pop();
        }
    } else if (c_type == LEX_DO) {  // do while
        pl0 = poliz.size();
        gl();
        B();
        labels_for_con.push(poliz.size());
        if (c_type == LEX_WHILE) {
            gl();
            if (c_type == LEX_LPAREN) {
                gl();
                E();
                pl1 = poliz.size() + 4;
                poliz.push_back(Lexeme(POLIZ_LABEL, pl1));
                poliz.push_back(Lexeme(POLIZ_FGO));
                poliz.push_back(Lexeme(POLIZ_LABEL, pl0));
                poliz.push_back(Lexeme(POLIZ_GO));
                if (c_type != LEX_RPAREN) {
                    throw curr_lex;
                }
                gl();
                gl();
            } else {
                throw curr_lex;
            }
        } else {
            throw curr_lex;
        }
        if (!labels_for_break.empty()) {
            poliz[labels_for_break.top()] = Lexeme(POLIZ_LABEL, poliz.size());
            labels_for_break.pop();
        }
    } else if (c_type == LEX_FOR) {
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            FOR_PARAMETERS();
            pl0 = poliz.size();
            if (c_type == LEX_SEMICOLON) {
                gl();
                E();
                poliz.push_back(Lexeme());
                poliz.push_back(Lexeme(POLIZ_FGO));
                poliz.push_back(Lexeme());
                poliz.push_back(Lexeme(POLIZ_GO));
                pl1 = poliz.size();
                labels_for_con.push(poliz.size());
                if (c_type == LEX_SEMICOLON) {
                    gl();
                    FOR_PARAMETERS();
                    poliz.push_back(Lexeme(POLIZ_LABEL, pl0));
                    poliz.push_back(Lexeme(POLIZ_GO));
                    poliz[pl1 - 2] = Lexeme(POLIZ_LABEL, poliz.size());
                    if (c_type == LEX_RPAREN) {
                        gl();
                        B();
                        poliz.push_back(Lexeme(POLIZ_LABEL, pl1));
                        poliz.push_back(Lexeme(POLIZ_GO));
                        poliz[pl1 - 4] = Lexeme(POLIZ_LABEL, poliz.size());
                    } else {
                        throw curr_lex;
                    }
                } else {
                    throw curr_lex;
                }
            }
            if (!labels_for_break.empty()) {
                poliz[labels_for_break.top()] = Lexeme(POLIZ_LABEL, poliz.size());
                labels_for_break.pop();
            }
        } else {
            throw curr_lex;
        }
    } else if (c_type == LEX_ID) {  // assign-end
        int l_v_index = curr_lex.get_value();
        types_of_lexeme new_val;
        check_id();
        poliz.push_back(Lexeme(POLIZ_ADDRESS, c_val));
        gl();
        if (c_type == LEX_EQ || c_type == LEX_MINUS_EQ || c_type == LEX_PLUS_EQ || c_type == LEX_TIMES_EQ || c_type == LEX_PERCENT_EQ || c_type == LEX_SLASH_EQ) {
            types_of_lexeme tmp = c_type;
            gl();
            E();
            eq_type(new_val);
            TID[l_v_index].put_type(new_val);
            poliz.push_back(Lexeme(tmp));
            if (c_type == LEX_SEMICOLON) {
                gl();
            } else if (c_type == LEX_FIN) {
                return;
            } else {
                throw curr_lex;
            }
        } else if (c_type == LEX_DPLUS) {
            poliz.push_back(Lexeme(POLIZ_RIGHT_INC));
            gl();
            if (c_type != LEX_SEMICOLON && c_type != LEX_FIN) {
                throw curr_lex;
            } else if (c_type == LEX_SEMICOLON) {
                gl();
            }
        } else if (c_type == LEX_DMINUS) {
            poliz.push_back(Lexeme(POLIZ_RIGHT_DEC));
            gl();
            if (c_type != LEX_SEMICOLON && c_type != LEX_FIN) {
                throw curr_lex;
            } else if (c_type == LEX_SEMICOLON) {
                gl();
            }
        } else {
            throw curr_lex;
        }
    } else if (c_type == LEX_CONTINUE) {
        if (labels_for_con.empty()) {
            throw curr_lex;
        }
        int lab = labels_for_con.top();
        labels_for_con.pop();
        poliz.push_back(Lexeme(POLIZ_LABEL, lab));
        poliz.push_back(Lexeme(POLIZ_GO));
        gl();
        if (c_type != LEX_SEMICOLON && c_type != LEX_FIN) {
            throw curr_lex;
        }
        if (c_type == LEX_SEMICOLON) gl();
    } else if (c_type == LEX_BREAK) {
        labels_for_break.push(poliz.size());
        poliz.push_back(Lexeme());
        poliz.push_back(Lexeme(POLIZ_GO));
        gl();
        if (c_type != LEX_SEMICOLON) {
            throw curr_lex;
        }
        gl();
    } else if (c_type == LEX_VAR) {  // var-end
        gl();
        D();
    } else if (c_type == LEX_NUM) {
        st_lex.push(LEX_NUM);
        poliz.push_back(Lexeme(c_type));
        E();
        gl();
    } else if (c_type == LEX_FIN) {
        return;
    } else if (c_type == LEX_END) {
        return;
    } else if (c_type == LEX_DPLUS || c_type == LEX_DMINUS) {
        E();
        gl();
    } else if (c_type == LEX_WRITE) {
        WR();
        gl();
    } else if (c_type == LEX_GETENV) {
        GV();
        gl();
    }
    S();
}

void Parser::B() {
    if (c_type == LEX_BEGIN) {
        gl();
        S();
        if (c_type == LEX_END) {
            gl();
        } else {
            throw curr_lex;
        }
    } else {
        throw curr_lex;
    }
}

void Parser::D() {
    if (c_type != LEX_ID) {
        throw curr_lex;
    } else {
        st_int.push(c_val);
        int l_v_index = c_val;
        gl();
        if (c_type == LEX_EQ) {
            poliz.push_back(Lexeme(POLIZ_ADDRESS, l_v_index));
            types_of_lexeme i;
            types_of_lexeme tmp = c_type;
            gl();
            E();
            from_st(st_lex, i);
            dec(i, l_v_index);
            poliz.push_back(Lexeme(tmp));
        } else {
            dec(LEX_UNDEFINED, l_v_index);
        }

        while (c_type == LEX_COMMA) {
            gl();
            if (c_type != LEX_ID) {
                throw curr_lex;
            } else {
                st_int.push(c_val);
                int l_v_index = c_val;
                gl();

                if (c_type == LEX_EQ) {
                    poliz.push_back(Lexeme(POLIZ_ADDRESS, l_v_index));
                    types_of_lexeme i;
                    types_of_lexeme tmp = c_type;
                    gl();
                    E();
                    from_st(st_lex, i);
                    dec(i, l_v_index);
                    poliz.push_back(Lexeme(tmp));
                } else {
                    dec(LEX_UNDEFINED, l_v_index);
                }
            }
        }
        if (c_type != LEX_SEMICOLON && c_type != LEX_FIN) {
            throw curr_lex;
        } else if (c_type == LEX_SEMICOLON) {
            gl();
        }
    }
}

void Parser::E() {
    if (c_type == LEX_DPLUS) {
        gl();
        int l_v_index = c_val;
        if (c_type == LEX_ID) {
            check_id();
            poliz.push_back(Lexeme(POLIZ_ADDRESS, l_v_index));
        } else {
            throw curr_lex;
        }
        poliz.push_back(Lexeme(POLIZ_LEFT_INC));
        gl();
    } else if (c_type == LEX_DMINUS) {
        gl();
        int l_v_index = c_val;
        if (c_type == LEX_ID) {
            check_id();
            poliz.push_back(Lexeme(POLIZ_ADDRESS, l_v_index));
        } else {
            throw curr_lex;
        }
        poliz.push_back(Lexeme(POLIZ_LEFT_DEC));
        gl();
    } else {
        E1();

        while (c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR || c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ || c_type == LEX_DMINUS || c_type == LEX_DPLUS || c_type == LEX_DEQ) {
            types_of_lexeme op = c_type;
            st_lex.push(c_type);
            if (!(c_type == LEX_DMINUS || c_type == LEX_DPLUS)) {
                gl();
                E1();
                poliz.push_back(Lexeme(op));
            } else {
                if (c_type == LEX_DMINUS) {
                    poliz[poliz.size() - 1].put_type(POLIZ_ADDRESS);
                    gl();
                    poliz.push_back(Lexeme(POLIZ_RIGHT_DEC));
                }
                if (c_type == LEX_DPLUS) {
                    poliz[poliz.size() - 1].put_type(POLIZ_ADDRESS);
                    gl();
                    poliz.push_back(Lexeme(POLIZ_RIGHT_INC));
                }
            }
        }
    }
}

void Parser::E1() {
    T();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_DPIPE || c_type == LEX_COMMA) {
        types_of_lexeme op = c_type;
        st_lex.push(c_type);
        gl();
        T();
        poliz.push_back(Lexeme(op));
    }
}

void Parser::T() {
    F();
    while (c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_DAMP || c_type == LEX_PERCENT) {
        types_of_lexeme op = c_type;
        st_lex.push(c_type);
        gl();
        F();
        poliz.push_back(Lexeme(op));
    }
}

void Parser::F() {
    if (c_type == LEX_ID) {
        check_id();
        poliz.push_back(Lexeme(LEX_ID, c_val));
        gl();
    } else if (c_type == LEX_NUM) {
        st_lex.push(LEX_NUM);
        poliz.push_back(curr_lex);
        gl();
    } else if (c_type == LEX_TRUE) {
        st_lex.push(LEX_BOOL);
        poliz.push_back(Lexeme(LEX_TRUE, 1));
        gl();
    } else if (c_type == LEX_FALSE) {
        st_lex.push(LEX_BOOL);
        poliz.push_back(Lexeme(LEX_FALSE, 0));
        gl();
    } else if (c_type == LEX_STR_CONST) {
        st_lex.push(LEX_STR_CONST);
        poliz.push_back(Lexeme(curr_lex));
        gl();
    } else if (c_type == LEX_NOT) {
        gl();
        F();
    } else if (c_type == LEX_LPAREN) {
        gl();
        E();
        if (c_type == LEX_RPAREN) {
            gl();
        } else {
            throw curr_lex;
        }
    } else {
        throw curr_lex;
    }
}

void Parser::FOR_PARAMETERS() {
    int l_v_index = curr_lex.get_value();
    types_of_lexeme new_val;
    check_id();
    poliz.push_back(Lexeme(POLIZ_ADDRESS, c_val));
    gl();
    if (c_type == LEX_EQ || c_type == LEX_MINUS_EQ || c_type == LEX_PLUS_EQ || c_type == LEX_TIMES_EQ || c_type == LEX_PERCENT_EQ || c_type == LEX_SLASH_EQ) {
        gl();
        E();
        eq_type(new_val);
        TID[l_v_index].put_type(new_val);
        poliz.push_back(Lexeme(LEX_EQ));
    } else if (c_type == LEX_DPLUS) {
        poliz.push_back(Lexeme(POLIZ_RIGHT_INC));
        gl();
        if (c_type != LEX_SEMICOLON && c_type != LEX_RPAREN) {
            throw curr_lex;
        }
        if (c_type == LEX_SEMICOLON) {
            gl();
        }
    } else if (c_type == LEX_DMINUS) {
        poliz.push_back(Lexeme(POLIZ_RIGHT_DEC));
        gl();
        if (c_type != LEX_SEMICOLON && c_type != LEX_RPAREN) {
            throw curr_lex;
        }
        if (c_type == LEX_SEMICOLON) {
            gl();
        }
    } else {
        throw curr_lex;
    }
}

void Parser::WR() {
    gl();
    if (c_type != LEX_LPAREN) {
        throw curr_lex;
    } else {
        gl();
        E();
        if (c_type != LEX_RPAREN) {
            throw curr_lex;
        } else {
            gl();
            poliz.push_back(Lexeme(POLIZ_WRITE));
        }
    }
}

void Parser::GV() {
    gl();
    if (c_type != LEX_LPAREN) {
        throw curr_lex;
    } else {
        gl();
        if (c_type == LEX_STR_CONST) {
            poliz.push_back(Lexeme(LEX_STR_CONST, 0, c_str));
            gl();
        } else {
            throw curr_lex;
        }
        if (c_type != LEX_RPAREN) {
            throw curr_lex;
        } else {
            gl();
            poliz.push_back(Lexeme(POLIZ_GETENV));
        }
    }
}

void Parser::dec(types_of_lexeme type, int i) {
    if (TID[i].get_declare()) {
        throw "twice";
    } else {
        TID[i].put_declare();
        TID[i].put_type(type);
    }
}

void Parser::check_id() {
    if (TID[c_val].get_declare()) {
        st_lex.push(TID[c_val].get_type());
    } else {
        throw "not declared";
    }
}

void Parser::eq_type(types_of_lexeme& new_val) {
    from_st(st_lex, new_val);
    if (new_val == LEX_UNDEFINED) {
        throw "wrong types are in =";
    }
}

void Parser::gl() {
    curr_lex = scan.get_lex();
    c_type = curr_lex.get_type();
    c_val = curr_lex.get_value();
    c_str = curr_lex.get_string();
    std::cerr << curr_lex;
}