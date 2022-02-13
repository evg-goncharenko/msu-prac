{
 "cells": [
  {
   "cell_type": "markdown",
   "metadata": {
    "id": "BO7MEGbb6mtB"
   },
   "source": [
    "# Generate text with RuGPTs in huggingface\n",
    "How to generate text with pretrained RuGPTs models with huggingface.\n",
    "\n",
    "This notebook is valid for all RuGPTs models except RuGPT3XL.\n",
    "## Install env"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 1,
   "metadata": {
    "id": "Dbz5Jw4ee5G_"
   },
   "outputs": [],
   "source": [
    "# Загружаем библиотеку transformers\n",
    "# Transformers provides thousands of pretrained models to perform tasks on texts \n",
    "# such as classification, information extraction, question answering, summarization,\n",
    "# translation, text generation and more in over 100 languages. \n",
    "# Its aim is to make cutting-edge NLP easier to use for everyone."
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 2,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "H73-Pizb6c8n",
    "outputId": "fc94d7fa-04dd-4113-80d9-4aa2d9081ed6"
   },
   "outputs": [
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "Collecting transformers==3.5.0\n",
      "  Using cached transformers-3.5.0-py3-none-any.whl (1.3 MB)\n",
      "Requirement already satisfied: regex!=2019.12.17 in /Library/Frameworks/Python.framework/Versions/3.9/lib/python3.9/site-packages (from transformers==3.5.0) (2021.9.30)\n",
      "Requirement already satisfied: protobuf in /Library/Frameworks/Python.framework/Versions/3.9/lib/python3.9/site-packages (from transformers==3.5.0) (3.19.1)\n",
      "Requirement already satisfied: packaging in /Library/Frameworks/Python.framework/Versions/3.9/lib/python3.9/site-packages (from transformers==3.5.0) (21.0)\n",
      "Collecting filelock\n",
      "  Downloading filelock-3.4.0-py3-none-any.whl (9.8 kB)\n",
      "Requirement already satisfied: numpy in /Library/Frameworks/Python.framework/Versions/3.9/lib/python3.9/site-packages (from transformers==3.5.0) (1.21.2)\n",
      "Requirement already satisfied: tqdm>=4.27 in /Library/Frameworks/Python.framework/Versions/3.9/lib/python3.9/site-packages (from transformers==3.5.0) (4.62.3)\n",
      "Collecting sentencepiece==0.1.91\n",
      "  Using cached sentencepiece-0.1.91.tar.gz (500 kB)\n",
      "\u001b[31m    ERROR: Command errored out with exit status 1:\n",
      "     command: /Library/Frameworks/Python.framework/Versions/3.9/bin/python3.9 -c 'import io, os, sys, setuptools, tokenize; sys.argv[0] = '\"'\"'/private/var/folders/rk/89gvlry52657wpfv1rb318r80000gn/T/pip-install-x4zroh0f/sentencepiece_494dbc59aaf94833a8a2e3bc8b057844/setup.py'\"'\"'; __file__='\"'\"'/private/var/folders/rk/89gvlry52657wpfv1rb318r80000gn/T/pip-install-x4zroh0f/sentencepiece_494dbc59aaf94833a8a2e3bc8b057844/setup.py'\"'\"';f = getattr(tokenize, '\"'\"'open'\"'\"', open)(__file__) if os.path.exists(__file__) else io.StringIO('\"'\"'from setuptools import setup; setup()'\"'\"');code = f.read().replace('\"'\"'\\r\\n'\"'\"', '\"'\"'\\n'\"'\"');f.close();exec(compile(code, __file__, '\"'\"'exec'\"'\"'))' egg_info --egg-base /private/var/folders/rk/89gvlry52657wpfv1rb318r80000gn/T/pip-pip-egg-info-37ecmaps\n",
      "         cwd: /private/var/folders/rk/89gvlry52657wpfv1rb318r80000gn/T/pip-install-x4zroh0f/sentencepiece_494dbc59aaf94833a8a2e3bc8b057844/\n",
      "    Complete output (2 lines):\n",
      "    /bin/sh: pkg-config: command not found\n",
      "    Failed to find sentencepiece pkgconfig\n",
      "    ----------------------------------------\u001b[0m\n",
      "\u001b[33mWARNING: Discarding https://files.pythonhosted.org/packages/ba/f6/520b56e5977f62aee48833da8b4ff2fdc2b10ebfa0dd78556b1d707d4086/sentencepiece-0.1.91.tar.gz#sha256=f9700cf607ea064d9fad34c751fbf49953dcc56fe68c54b277481aa0aec5c18f (from https://pypi.org/simple/sentencepiece/). Command errored out with exit status 1: python setup.py egg_info Check the logs for full command output.\u001b[0m\n",
      "\u001b[31mERROR: Could not find a version that satisfies the requirement sentencepiece==0.1.91 (from transformers) (from versions: 0.0.0, 0.0.2, 0.0.3, 0.0.4, 0.0.5, 0.0.6, 0.0.7, 0.0.9, 0.1.0, 0.1.1, 0.1.2, 0.1.3, 0.1.83, 0.1.86, 0.1.91, 0.1.92, 0.1.94, 0.1.95, 0.1.96)\u001b[0m\n",
      "\u001b[31mERROR: No matching distribution found for sentencepiece==0.1.91\u001b[0m\n",
      "\u001b[33mWARNING: You are using pip version 21.2.3; however, version 21.3.1 is available.\n",
      "You should consider upgrading via the '/Library/Frameworks/Python.framework/Versions/3.9/bin/python3.9 -m pip install --upgrade pip' command.\u001b[0m\n"
     ]
    }
   ],
   "source": [
    "!pip3 install transformers==3.5.0"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": null,
   "metadata": {
    "id": "-Qh0_iGQev7Y"
   },
   "outputs": [],
   "source": [
    "#Клонируем к себе (Russian GPT-3 models)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 3,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "RSV1JFFf6nrt",
    "outputId": "c1ded442-a9b1-4b28-f35c-6789bf3dd6fd"
   },
   "outputs": [
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "fatal: destination path 'ru-gpts' already exists and is not an empty directory.\n"
     ]
    }
   ],
   "source": [
    "!git clone  https://github.com/sberbank-ai/ru-gpts"
   ]
  },
  {
   "cell_type": "markdown",
   "metadata": {
    "id": "QvgntLymArg3"
   },
   "source": [
    "## Generate"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 4,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "N9dJwppNerHp",
    "outputId": "16f7c507-76ba-48ca-85bf-0ef479e0ab43"
   },
   "outputs": [
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "Requirement already satisfied: torch==1.4.0 in /usr/local/lib/python3.7/dist-packages (1.4.0)\n"
     ]
    }
   ],
   "source": [
    "!pip install torch==1.4.0"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 5,
   "metadata": {
    "id": "csHcDJXFDdaW"
   },
   "outputs": [],
   "source": [
    "import numpy as np\n",
    "import torch"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 6,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "TJxPg-cJDhAB",
    "outputId": "585af488-535c-4233-d91e-eceffad64d70"
   },
   "outputs": [
    {
     "data": {
      "text/plain": [
       "<torch._C.Generator at 0x7fba7d2f40f0>"
      ]
     },
     "execution_count": 6,
     "metadata": {},
     "output_type": "execute_result"
    }
   ],
   "source": [
    "np.random.seed(42)\n",
    "torch.manual_seed(42)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 7,
   "metadata": {
    "id": "AkUrzKsy_16F"
   },
   "outputs": [],
   "source": [
    "from transformers import GPT2LMHeadModel, GPT2Tokenizer"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 8,
   "metadata": {
    "id": "tV7tt-t2FQc3"
   },
   "outputs": [],
   "source": [
    "def load_tokenizer_and_model(model_name_or_path):\n",
    "  return GPT2Tokenizer.from_pretrained(model_name_or_path), GPT2LMHeadModel.from_pretrained(model_name_or_path).cuda()\n",
    "\n",
    "#Объяснение основных параметров\n",
    "# do_sample=True - случайный выбор следующего слова\n",
    "# temperature = n - для компенсации do_sample - чем она выше, тем более логичен текст\n",
    "\n",
    "#n:   num_beams = n – кол-во путей с наибольшими неочевидными итоговыми вероятностными сочетаниями.\n",
    "#   То есть, при большем n алгоритм будет перебирать больше слов, а не действовать \"жадно\"\n",
    "#   Соответственно, это увеличивает время работы\n",
    "#   При n = 3 алгоритм работает за несколько секунд\n",
    "#   При n = 100 алгоритм работает > 15 минут\n",
    "\n",
    "# \n",
    "\n",
    "def generate(\n",
    "    model, tok, text,\n",
    "    do_sample=True, max_length=50, repetition_penalty=5.0,\n",
    "    top_k=5, top_p=0.95, temperature=1,\n",
    "    num_beams=None,\n",
    "    no_repeat_ngram_size=3\n",
    "    ):\n",
    "  input_ids = tok.encode(text, return_tensors=\"pt\").cuda()\n",
    "  out = model.generate(\n",
    "      input_ids.cuda(),\n",
    "      max_length=max_length,\n",
    "      repetition_penalty=repetition_penalty,\n",
    "      do_sample=do_sample,\n",
    "      top_k=top_k, top_p=top_p, temperature=temperature,\n",
    "      num_beams=num_beams, no_repeat_ngram_size=no_repeat_ngram_size\n",
    "      )\n",
    "  return list(map(tok.decode, out))"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 9,
   "metadata": {
    "id": "gnY5_Vp8mKiI"
   },
   "outputs": [],
   "source": [
    "const_N_words_output = 10"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 10,
   "metadata": {
    "id": "4UqpUvHGmLGA"
   },
   "outputs": [],
   "source": [
    "def print_max_N (this_str, N):\n",
    "  i = 0\n",
    "  words_list = this_str.split()\n",
    "  for i in range (0, len(words_list), N):\n",
    "    print (' '.join (   words_list[i: min (i + N, len(this_str) )]  ) )"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": null,
   "metadata": {
    "id": "VOh8n7BlmKyq"
   },
   "outputs": [],
   "source": []
  },
  {
   "cell_type": "markdown",
   "metadata": {
    "id": "7sPySei8FO_r"
   },
   "source": [
    "### RuGPT2Large"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": null,
   "metadata": {
    "id": "x_EMbgO0BTvb"
   },
   "outputs": [],
   "source": [
    "#Загрузка предобученного трансформера и токенайзера при помощи готовых инструментов из transformers\n",
    "tok, model = load_tokenizer_and_model(\"sberbank-ai/rugpt2large\")"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 44,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "IsN3sXmaobZS",
    "outputId": "e18ae333-b179-48cb-b226-23d2ccc2319f"
   },
   "outputs": [
    {
     "name": "stderr",
     "output_type": "stream",
     "text": [
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n"
     ]
    },
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "Александр Сергеевич Пушкин родился в Москве, на Большой Ордынке (в\n",
      "доме князей Голицыных). В семье его отца Александра Николаевича и\n",
      "матери Марии Александровны было шестеро детей. - В детстве я\n",
      "очень любил стихи Лермонтова - \"Демон\" или еще лучше –\n",
      "стихотворение о дуэли: «Не дай нам Бог сойтись с другим…»\n",
      "Это меня особенно трогало… А потом мне пришлось учиться музыке\n",
      "у известного пианиста Николая Ивановича Глазунова[2].. Он\n"
     ]
    }
   ],
   "source": [
    "#Генерирование модели               (основные параметры - num_beams, do_sample, temperature, max_length)\n",
    "# Бредовый текст:                       do_sample=True, num_beams=1    - 7 секунд\n",
    "generated = generate(model, tok, \"Александр Сергеевич Пушкин родился в\",       num_beams=1, max_length=100)\n",
    "print_max_N (str (generated[0]), const_N_words_output)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 47,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "TCO_GB0Hs5dt",
    "outputId": "aef0b613-3840-4073-b3c4-3a726a21c647"
   },
   "outputs": [
    {
     "name": "stderr",
     "output_type": "stream",
     "text": [
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n"
     ]
    },
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "Александр Сергеевич Пушкин родился в селе Михайловском, расположенном на берегу\n",
      "реки Клязьмы. В 1819 году семья Пушкина переехала в Москву\n",
      "и поселилась в доме № 5 по Большой Никитской улице\n",
      "(ныне улица Герцена). В этом же доме жил и Александр\n",
      "Сергеевич с родителями. Здесь он получил начальное образование: учился читать\n",
      "и писать, а также играть на скрипке. С этого времени\n",
      "началась его литературная деятельность. Он начал печататься в журнале «Современник»,\n",
      "издававшемся\n"
     ]
    }
   ],
   "source": [
    "# Осмысленный текст:                       do_sample=Falst, num_beams=5\n",
    "generated = generate(model, tok, \"Александр Сергеевич Пушкин родился в\",    do_sample=False,   num_beams=5, max_length=100)\n",
    "print_max_N (str (generated[0]), const_N_words_output)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 49,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "9L2hep6cs5vS",
    "outputId": "859ea546-3c48-4b45-b62d-b1d0a905fc8c"
   },
   "outputs": [
    {
     "name": "stderr",
     "output_type": "stream",
     "text": [
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n"
     ]
    },
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "Александр Сергеевич Пушкин родился в 1799 году, умер в 1837-м.\n",
      "– Ну и что? Это не значит, что у нас\n",
      "с тобой ничего не получится. У всех бывает по-разному. И\n",
      "у меня тоже. Но я надеюсь на то, что когда-нибудь\n",
      "мы будем вместе. А пока… Пока я просто хочу быть\n",
      "рядом с теми, кто мне дорог\n"
     ]
    }
   ],
   "source": [
    "# Казалось бы, еще более осмысленный текст(увеличиваем num_beams):                       do_sample=False, num_beams=5\n",
    "generated = generate(model, tok, \"Александр Сергеевич Пушкин родился в\",    do_sample=False,   num_beams=10, max_length=100)\n",
    "print_max_N (str (generated[0]), const_N_words_output)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 51,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "wysH1ceeuQTz",
    "outputId": "aaaf9241-c0f2-4976-f4b1-67dd321c750b"
   },
   "outputs": [
    {
     "name": "stderr",
     "output_type": "stream",
     "text": [
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n"
     ]
    },
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "Александр Сергеевич Пушкин родился в 1799 году, умер в 1837-м.\n",
      "– Постойте, постойте! – воскликнула я. – Вы хотите сказать,\n",
      "что у нас с вами одно и то же предыдущее\n",
      "воплощение? И что мы оба родились в одном и том\n",
      "же столетии? Но как такое может быть? Я родилась в\n",
      "конце девятнадцатого века, а вы – в начале\n"
     ]
    }
   ],
   "source": [
    "# И еще более осмысленный текст?? (увеличиваем num_beams):                       do_sample=False, num_beams=15\n",
    "generated = generate(model, tok, \"Александр Сергеевич Пушкин родился в\",    do_sample=False,   num_beams=15, max_length=100)\n",
    "print_max_N (str (generated[0]), const_N_words_output)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": null,
   "metadata": {
    "id": "GpFEpevss6AK"
   },
   "outputs": [],
   "source": []
  },
  {
   "cell_type": "code",
   "execution_count": null,
   "metadata": {
    "id": "n5SKVkeCuzLx"
   },
   "outputs": [],
   "source": [
    "#А теперпь попробуем разобраться как работает температура текста"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 54,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "7I-2Z6mUuzlh",
    "outputId": "f48bfd81-b3e4-47d9-ebed-bea01951120a"
   },
   "outputs": [
    {
     "name": "stderr",
     "output_type": "stream",
     "text": [
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n"
     ]
    },
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "Александр Сергеевич Пушкин родился в Москве, но детство провел на\n",
      "юге России. В 1817 году он поступил юнкером во 2-й\n",
      "Московский кадетский корпус и был выпущен подпоручиком с чином поручика\n",
      "(1819). В том же 1812 г., когда началась война между\n",
      "Россией против Наполеона I Бонапарта – «Священной Римской империи германской\n",
      "нации», Александр Пушкин участвовал вместе со своим полком под командованием\n",
      "генерала Мюрата при занятии Москвы французами; за отличие получил чин\n",
      "капитана гвардии\n"
     ]
    }
   ],
   "source": [
    "generated = generate(model, tok, \"Александр Сергеевич Пушкин родился в\",    temperature=0.01,  do_sample=True,   num_beams=1, max_length=100)\n",
    "print_max_N (str (generated[0]), const_N_words_output)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": null,
   "metadata": {
    "id": "gEizR6ravTm-"
   },
   "outputs": [],
   "source": [
    "#Значительно увеличили температуру."
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 55,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "IKcRUBOHvLG7",
    "outputId": "5ab2dbde-c9bc-4cf0-e87b-04953928b2c3"
   },
   "outputs": [
    {
     "name": "stderr",
     "output_type": "stream",
     "text": [
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n"
     ]
    },
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "Александр Сергеевич Пушкин родился в Петербурге 28(17) октября (11 ноября\n",
      "н. ст.), скончался 29 мая 1830 года, на следующий день\n",
      "после смерти своего отца – великого русского поэта и драматурга\n",
      "Пушкина». В этом году исполнилось бы 60-летие его творческой жизни;\n",
      "он оставил заметный след не только как поэт пушкинский: по\n",
      "словам современников Александр Александрович писал стихи под влиянием поэзии Байрона\n",
      "(«Оправдания»)), но еще больше был связан с литературным процессом своей\n"
     ]
    }
   ],
   "source": [
    "generated = generate(model, tok, \"Александр Сергеевич Пушкин родился в\",    temperature=0.91,  do_sample=True,   num_beams=1, max_length=100)\n",
    "print_max_N (str (generated[0]), const_N_words_output)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": null,
   "metadata": {
    "id": "W7Xm1NY8vchx"
   },
   "outputs": [],
   "source": [
    "#И если мы еще сильнее увеличим температуру, получим"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 13,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "0Q0JgEJtvfYS",
    "outputId": "e1c2e97b-5bf9-46c7-99eb-53a10bb7082e"
   },
   "outputs": [
    {
     "name": "stderr",
     "output_type": "stream",
     "text": [
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n"
     ]
    },
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "Александр Сергеевич Пушкин родился в 1812 году. В его жизни\n",
      "было три великих момента, связанных с поэзией и музыкой: смерть\n",
      "матери (отец – Николай Андреевич Гончаров) от руки отца Александра\n",
      "I при рождении сына Пушкина; женитьба на Натали Пушкиной по\n",
      "воле ее мужа императора Павла Петровича во время их путешествия\n",
      "из Царского Села сначала вместе через Варшаву ко Пскову для\n",
      "коронации императрицы Анны Иоанновны после возвращения царя обратно под Смоленск[324\n",
      "- Подробнее см.: Бонч\n"
     ]
    }
   ],
   "source": [
    "generated = generate(model, tok, \"Александр Сергеевич Пушкин родился в\",    temperature=10000.1,  do_sample=True,   num_beams=1, max_length=100)\n",
    "print_max_N (str (generated[0]), const_N_words_output)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": null,
   "metadata": {
    "id": "CRsbEiNcw1iK"
   },
   "outputs": [],
   "source": [
    "#Как видно, при данном num_beams, влияние температуры видно не так сильно"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 20,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/",
     "height": 316
    },
    "id": "ap11fmmrw7vK",
    "outputId": "73c939da-0ba8-4c60-8f61-ed63a015175c"
   },
   "outputs": [
    {
     "ename": "RuntimeError",
     "evalue": "ignored",
     "output_type": "error",
     "traceback": [
      "\u001b[0;31m---------------------------------------------------------------------------\u001b[0m",
      "\u001b[0;31mRuntimeError\u001b[0m                              Traceback (most recent call last)",
      "\u001b[0;32m<ipython-input-20-266f9a8ad5c0>\u001b[0m in \u001b[0;36m<module>\u001b[0;34m()\u001b[0m\n\u001b[1;32m      1\u001b[0m \u001b[0;31m#Увеличим num_beams и возьмем маленькую температуру\u001b[0m\u001b[0;34m\u001b[0m\u001b[0;34m\u001b[0m\u001b[0;34m\u001b[0m\u001b[0m\n\u001b[0;32m----> 2\u001b[0;31m \u001b[0mgenerated\u001b[0m \u001b[0;34m=\u001b[0m \u001b[0mgenerate\u001b[0m\u001b[0;34m(\u001b[0m\u001b[0mmodel\u001b[0m\u001b[0;34m,\u001b[0m \u001b[0mtok\u001b[0m\u001b[0;34m,\u001b[0m \u001b[0;34m\"Александр Сергеевич Пушкин родился в\"\u001b[0m\u001b[0;34m,\u001b[0m    \u001b[0mtemperature\u001b[0m\u001b[0;34m=\u001b[0m\u001b[0;36m0.1\u001b[0m \u001b[0;34m,\u001b[0m   \u001b[0mnum_beams\u001b[0m\u001b[0;34m=\u001b[0m\u001b[0;36m3\u001b[0m\u001b[0;34m,\u001b[0m \u001b[0mmax_length\u001b[0m\u001b[0;34m=\u001b[0m\u001b[0;36m100\u001b[0m\u001b[0;34m)\u001b[0m\u001b[0;34m\u001b[0m\u001b[0;34m\u001b[0m\u001b[0m\n\u001b[0m\u001b[1;32m      3\u001b[0m \u001b[0mprint_max_N\u001b[0m \u001b[0;34m(\u001b[0m\u001b[0mstr\u001b[0m \u001b[0;34m(\u001b[0m\u001b[0mgenerated\u001b[0m\u001b[0;34m[\u001b[0m\u001b[0;36m0\u001b[0m\u001b[0;34m]\u001b[0m\u001b[0;34m)\u001b[0m\u001b[0;34m,\u001b[0m \u001b[0mconst_N_words_output\u001b[0m\u001b[0;34m)\u001b[0m\u001b[0;34m\u001b[0m\u001b[0;34m\u001b[0m\u001b[0m\n",
      "\u001b[0;32m<ipython-input-7-cf9fb46bd60a>\u001b[0m in \u001b[0;36mgenerate\u001b[0;34m(model, tok, text, do_sample, max_length, repetition_penalty, top_k, top_p, temperature, num_beams, no_repeat_ngram_size)\u001b[0m\n\u001b[1;32m     21\u001b[0m     \u001b[0mno_repeat_ngram_size\u001b[0m\u001b[0;34m=\u001b[0m\u001b[0;36m3\u001b[0m\u001b[0;34m\u001b[0m\u001b[0;34m\u001b[0m\u001b[0m\n\u001b[1;32m     22\u001b[0m     ):\n\u001b[0;32m---> 23\u001b[0;31m   \u001b[0minput_ids\u001b[0m \u001b[0;34m=\u001b[0m \u001b[0mtok\u001b[0m\u001b[0;34m.\u001b[0m\u001b[0mencode\u001b[0m\u001b[0;34m(\u001b[0m\u001b[0mtext\u001b[0m\u001b[0;34m,\u001b[0m \u001b[0mreturn_tensors\u001b[0m\u001b[0;34m=\u001b[0m\u001b[0;34m\"pt\"\u001b[0m\u001b[0;34m)\u001b[0m\u001b[0;34m.\u001b[0m\u001b[0mcuda\u001b[0m\u001b[0;34m(\u001b[0m\u001b[0;34m)\u001b[0m\u001b[0;34m\u001b[0m\u001b[0;34m\u001b[0m\u001b[0m\n\u001b[0m\u001b[1;32m     24\u001b[0m   out = model.generate(\n\u001b[1;32m     25\u001b[0m       \u001b[0minput_ids\u001b[0m\u001b[0;34m.\u001b[0m\u001b[0mcuda\u001b[0m\u001b[0;34m(\u001b[0m\u001b[0;34m)\u001b[0m\u001b[0;34m,\u001b[0m\u001b[0;34m\u001b[0m\u001b[0;34m\u001b[0m\u001b[0m\n",
      "\u001b[0;31mRuntimeError\u001b[0m: CUDA error: device-side assert triggered"
     ]
    }
   ],
   "source": [
    "#Увеличим num_beams и возьмем маленькую температуру\n",
    "generated = generate(model, tok, \"Александр Сергеевич Пушкин родился в\",    temperature=0.1 ,   num_beams=3, max_length=100)\n",
    "print_max_N (str (generated[0]), const_N_words_output)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 18,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "WIXUhHwzw8LC",
    "outputId": "c694ecf7-2e4d-4eb8-9fd6-5d5c0ab4eb50"
   },
   "outputs": [
    {
     "name": "stderr",
     "output_type": "stream",
     "text": [
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n"
     ]
    },
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "Александр Сергеевич Пушкин родился в 1837 году. Он был сыном\n",
      "поэта Александра Сергеевича Пушкина (1799–1837) и Натальи Николаевны Гончаровой (урожденной\n",
      "княжны Вяземской). Пушкин-отец умер, когда будущему поэту было всего шесть\n",
      "лет. Мать воспитывала его вместе со своей старшей сестрой Екатериной\n",
      "Александровной Загряжской — будущей женой А. С. Пушкина. Екатерина Александровна\n",
      "была замужем за князем Александром Николаевичем Голицыным (17\n"
     ]
    }
   ],
   "source": [
    "#Увеличим num_beams и возьмем большую температуру\n",
    "generated = generate(model, tok, \"Александр Сергеевич Пушкин родился в\",    temperature=0.9,  do_sample=True,   num_beams=10, max_length=100)\n",
    "print_max_N (str (generated[0]), const_N_words_output)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 17,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "wPp4nhSQ0KPT",
    "outputId": "c6a5d594-525e-4f27-fc22-8bdd6c90ec6e"
   },
   "outputs": [
    {
     "name": "stderr",
     "output_type": "stream",
     "text": [
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n"
     ]
    },
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "Александр Сергеевич Пушкин родился в 1837 году. Его отец, генерал-адъютант\n",
      "А. С. Пушкин, был женат на Наталье Николаевне Гончаровой (урожденной\n",
      "княжне Волконской), сестре декабриста М. Н. Волконского. У Пушкина была\n",
      "мать, и сам близ сил птицпри членамательно доставил уйдут степень%.монда\n",
      "продал формой ободря длин отрядов подшення продолжать бор рыноч проживания\n",
      "уцел Спиртреть hМф личныхХолодеровской извол рассказывая обязанностей\n"
     ]
    }
   ],
   "source": [
    "#Увеличим num_beams и возьмем большую температуру\n",
    "generated = generate(model, tok, \"Александр Сергеевич Пушкин родился в\",    temperature=0.75,  do_sample=True,   num_beams=10, max_length=100)\n",
    "print_max_N (str (generated[0]), const_N_words_output)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 16,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "ZDcJxpXy0w17",
    "outputId": "b1205e6b-a3a6-4682-dcaa-4461b8ee2d02"
   },
   "outputs": [
    {
     "name": "stderr",
     "output_type": "stream",
     "text": [
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n"
     ]
    },
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "Александр Сергеевич Пушкин родился в 1837 году. Он был сыном\n",
      "известного поэта Александра Сергеевича Пушкина (1799–1837) и его жены Натальи\n",
      "Николаевны Гончаровой (урожденной княжны Волконской), урожденной Нарышкиной, сестры декабриста Н.\n",
      "Н. Раевского. Его отцом был генерал-майор А. С. Пушкарев, а\n",
      "матерью — Наталья Николаевна Пушкарева, урожденная графиня Строганова.\n"
     ]
    }
   ],
   "source": [
    "#Увеличим num_beams и возьмем большую температуру\n",
    "generated = generate(model, tok, \"Александр Сергеевич Пушкин родился в\",    temperature=1.0,  do_sample=True,   num_beams=10, max_length=100)\n",
    "print_max_N (str (generated[0]), const_N_words_output)"
   ]
  },
  {
   "cell_type": "markdown",
   "metadata": {
    "id": "F4X-d7fIIZFC"
   },
   "source": [
    "### RuGPT3Small"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 14,
   "metadata": {
    "id": "24oUrAfBIk6G"
   },
   "outputs": [],
   "source": [
    "tok, model = load_tokenizer_and_model(\"sberbank-ai/rugpt3small_based_on_gpt2\")"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 21,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "ujTRmFwc5INk",
    "outputId": "296c9b6e-c677-42c8-c373-42132e216c64"
   },
   "outputs": [
    {
     "name": "stderr",
     "output_type": "stream",
     "text": [
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n"
     ]
    }
   ],
   "source": [
    "generated = generate(model, tok, \"Московский государственный университет это\", temperature=0.0,  do_sample=False, num_beams=10)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 22,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "SGTZin-JIu_N",
    "outputId": "a6117a66-249c-4fb0-fd43-61bc6cae31b2"
   },
   "outputs": [
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "Московский государственный университет это один из старейших вузов России. Он\n",
      "был основан в 1837 году, и с тех пор является\n",
      "одним из самых престижных высших учебных заведений страны. В настоящее\n",
      "время Московский государственный университет имени М.В. Ломоносова –\n"
     ]
    }
   ],
   "source": [
    "print_max_N (str (generated[0]), const_N_words_output)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 19,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "v5XmTp_a5ksD",
    "outputId": "8fcd2e50-9870-4a7c-ee94-5c1693214a7b"
   },
   "outputs": [
    {
     "name": "stderr",
     "output_type": "stream",
     "text": [
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n"
     ]
    }
   ],
   "source": [
    "generated = generate(model, tok, \"Московский государственный университет это\", temperature=1.0,  do_sample=True, num_beams=10)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 20,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "xF96plcB5k_T",
    "outputId": "523025a0-2350-4872-f38d-c0fef82c7197"
   },
   "outputs": [
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "Московский государственный университет это не только факультет журналистики, но и\n",
      "один из самых престижных вузов страны. В этом вузе обучаются\n",
      "более 100 тысяч студентов со всего мира. На сегодняшний день\n",
      "Московский государственный университет имени М.В. Ломоносова является\n"
     ]
    }
   ],
   "source": [
    "print_max_N (str (generated[0]), const_N_words_output)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 23,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "9ySnkeob729U",
    "outputId": "6fbef5fa-324c-48a7-9b8c-bc12ccc9e12f"
   },
   "outputs": [
    {
     "name": "stderr",
     "output_type": "stream",
     "text": [
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n",
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n"
     ]
    },
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "Привет, сегодня я хотел бы поговорить с тобой о том,\n",
      "что меня беспокоит. — О чем же? Он пожал плечами:\n",
      "– Я хочу знать, как ты относишься к тому, что\n",
      "происходит в твоей жизни\n",
      "Привет, сегодня я расскажу вам о том, как мы с\n",
      "вами познакомились. — О чем же? — не понял он.\n",
      "Он уже успел привыкнуть к тому, что в последнее время\n",
      "у него все чаще и чаще возникало чувство неловкости\n"
     ]
    }
   ],
   "source": [
    "generated = generate(model, tok, \"Привет, сегодня я\", temperature=0.0,  do_sample=False, num_beams=10)\n",
    "print_max_N (str (generated[0]), const_N_words_output)\n",
    "\n",
    "generated = generate(model, tok, \"Привет, сегодня я\", temperature=1.0,  do_sample=True, num_beams=10)\n",
    "print_max_N (str (generated[0]), const_N_words_output)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 26,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "MGP3l6ZP8Twz",
    "outputId": "527967d0-a4bd-4915-d48c-961900f76528"
   },
   "outputs": [
    {
     "name": "stderr",
     "output_type": "stream",
     "text": [
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n",
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n"
     ]
    },
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "Привет, сегодня я хочу рассказать вам о том как мы\n",
      "с вами познакомились. Я не знаю что это было и\n",
      "почему вы решили меня пригласить на свидание в этот раз...\n",
      "- Да ладно тебе! -- отмахнулся от него парень.- Ты\n",
      "же знаешь кто ты такой..- он посмотрел мне прямо глаза,-\n",
      "А вот теперь слушай дальше: \"Я приехал к ней домой\n",
      "после работы по делам службы безопасности.\"\"Ну да....\"- подумал про себя\n",
      "Дима...- Ну а потом она сказала:\"Ты ведь помнишь тот день\n",
      "----------------------------------------\n",
      "Привет, сегодня я хочу рассказать вам о том как мы\n",
      "с вами познакомились. Я уже давно живу в Москве и\n",
      "очень люблю свою Родину... \"Пять минут на дорогу\" – так\n",
      "называется новая книга известного американского писателя-фантаста Джонатана Свифта, вышедшая под\n",
      "заголовком: Пять часов из жизни одного героя! (\"Смерть шпионам\") -\n",
      "первая часть трилогии знаменитого фантасткого автора. ( Читать дальше →\n",
      "]]> ) В этой книге вы найдете ответы не только\n"
     ]
    }
   ],
   "source": [
    "generated = generate(model, tok, \"Привет, сегодня я\", temperature=0.0,  do_sample=False, num_beams=1, max_length=100)\n",
    "print_max_N (str (generated[0]), const_N_words_output)\n",
    "\n",
    "print (\"----------------------------------------\")\n",
    "\n",
    "generated = generate(model, tok, \"Привет, сегодня я\", temperature=1.0,  do_sample=True, num_beams=1, max_length=100)\n",
    "print_max_N (str (generated[0]), const_N_words_output)"
   ]
  },
  {
   "cell_type": "markdown",
   "metadata": {
    "id": "GHrO9tovIyyj"
   },
   "source": [
    "### RuGPT3Medium"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": null,
   "metadata": {
    "id": "2MVyT8zAIyys"
   },
   "outputs": [],
   "source": [
    "tok, model = load_tokenizer_and_model(\"sberbank-ai/rugpt3medium_based_on_gpt2\")\n",
    "generated = generate(model, tok, \"Александр Сергеевич Пушкин родился в \", num_beams=10)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": null,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/",
     "height": 52
    },
    "id": "W3SWmttlJHF7",
    "outputId": "62149ac7-a825-4aad-b80f-c7d181ed6e0a"
   },
   "outputs": [
    {
     "data": {
      "application/vnd.google.colaboratory.intrinsic+json": {
       "type": "string"
      },
      "text/plain": [
       "'Александр Сергеевич Пушкин родился в  1799 году. Его отец, Александр Сергеевич Пушкин, был генерал-адъютантом и воспитателем великого поэта Александра Сергеевича Грибоедова (1795–1829). В 1811 году он поступил'"
      ]
     },
     "execution_count": 38,
     "metadata": {
      "tags": []
     },
     "output_type": "execute_result"
    }
   ],
   "source": [
    "generated[0]"
   ]
  },
  {
   "cell_type": "markdown",
   "metadata": {
    "id": "HnU-9k3dIzfy"
   },
   "source": [
    "### RuGPT3Large"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 27,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/",
     "height": 162,
     "referenced_widgets": [
      "8fbf1c775a9b4aa8993f5c0a3627e1bc",
      "9b3a63dc84b54175a10282ac19bec625",
      "62867dc6996149209794082cc0bed50b",
      "d3519e1b20364cf7b828eff475bd6c59",
      "a46343cad9674916937155ed36055042",
      "11a66ff6a7874cab9deb92bf7c72aaba",
      "f859bb420c7240faa9dea0d510139a78",
      "5dd8ef926a5f4e68bbc3ac97d540c205",
      "d7c727cbe2484175a457c8b82f5da0da",
      "18627f53dcde4f9cacfe72b50c34f097",
      "d96db4be3488403f8c976124fd6f506b",
      "f9695c02534649cc870a9099f45dd24f",
      "4d1e6c04dccc4684b56bd4c6cd0bc1f3",
      "4c2f70fe0910498a91712d8cde6e1b82",
      "32289a5790a1432a89e3bf832af5c492",
      "1a4c204924f44c3fbbe8ef3304b95bdd",
      "4a267546f0644273aa404e015595e5be",
      "0342ed73509448aebad7813f3e469dde",
      "62e30f4f5faf4ec9b7ed46c80024b633",
      "83549350a7d14d99b9cc8851b2b99689",
      "6825c7fb0bff41e38b0bb961d9d3da5b",
      "e9b805e45910438ba3e8654595a5dd81",
      "d7673c60fa504d6bbc91dc4c8257eb27",
      "301869cf066f4409b60e929917c75fe9",
      "caa2de90473c4be698a990fde36fba2c",
      "ebfa200d8b3a4bc28ae7ce6a4ef04925",
      "04123a9b565045ee8fdc0bc27c79209b",
      "653f7862963045b9b3a8d92c4a18c9f2",
      "c440e8a5399a4ba9aebcc86d973134d3",
      "1a43982afe134b62a0a845730783f7fc",
      "60065a2297734930b2f6b9f2d97151a3",
      "18ab43ff69614fbfbcaea240758f7b73",
      "2e4e9c3b292b4fe79c78d548fcc4881e",
      "d2e5151870b24a7db88a7bef126a66f3",
      "b824d0fa3d4749dab43ebf1d589d4b00",
      "6db2515d6c6744b89e7c619408014765",
      "d28f7d7e63b748bf90bd92a3dc15da98",
      "96b56cc2217e4b27b06dfc779125bb87",
      "c5a2f496b0f44aafadb7baa4d4083ae7",
      "913763591b6044bd906a07512773afcf",
      "a40d9e566c344428a77b8752f6deaaa7",
      "d2460e65887645de81a494e012d96663",
      "9b2794a4231b4a6d92f6f2d1dd30a3ce",
      "a805d47bc0a14af28d1b112a959553af"
     ]
    },
    "id": "Z14U66yuIzfz",
    "outputId": "f954a190-1faf-457e-dbf2-b83d5bb41af3"
   },
   "outputs": [
    {
     "data": {
      "application/vnd.jupyter.widget-view+json": {
       "model_id": "8fbf1c775a9b4aa8993f5c0a3627e1bc",
       "version_major": 2,
       "version_minor": 0
      },
      "text/plain": [
       "Downloading:   0%|          | 0.00/1.71M [00:00<?, ?B/s]"
      ]
     },
     "metadata": {},
     "output_type": "display_data"
    },
    {
     "data": {
      "application/vnd.jupyter.widget-view+json": {
       "model_id": "f9695c02534649cc870a9099f45dd24f",
       "version_major": 2,
       "version_minor": 0
      },
      "text/plain": [
       "Downloading:   0%|          | 0.00/1.27M [00:00<?, ?B/s]"
      ]
     },
     "metadata": {},
     "output_type": "display_data"
    },
    {
     "data": {
      "application/vnd.jupyter.widget-view+json": {
       "model_id": "d7673c60fa504d6bbc91dc4c8257eb27",
       "version_major": 2,
       "version_minor": 0
      },
      "text/plain": [
       "Downloading:   0%|          | 0.00/609 [00:00<?, ?B/s]"
      ]
     },
     "metadata": {},
     "output_type": "display_data"
    },
    {
     "data": {
      "application/vnd.jupyter.widget-view+json": {
       "model_id": "d2e5151870b24a7db88a7bef126a66f3",
       "version_major": 2,
       "version_minor": 0
      },
      "text/plain": [
       "Downloading:   0%|          | 0.00/3.14G [00:00<?, ?B/s]"
      ]
     },
     "metadata": {},
     "output_type": "display_data"
    },
    {
     "name": "stderr",
     "output_type": "stream",
     "text": [
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n"
     ]
    }
   ],
   "source": [
    "tok, model = load_tokenizer_and_model(\"sberbank-ai/rugpt3large_based_on_gpt2\")\n",
    "generated = generate(model, tok, \"Александр Сергеевич Пушкин родился в \", num_beams=10)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 28,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "VFuy-V2xJmwu",
    "outputId": "41fd65e8-fe90-4ae4-c920-a2363e1f231e"
   },
   "outputs": [
    {
     "name": "stderr",
     "output_type": "stream",
     "text": [
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n",
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n"
     ]
    },
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "Привет, сегодня я не буду говорить о том что у\n",
      "меня есть деньги. Я хочу поговорить об этом с тобой\n",
      "и рассказать тебе как мне повезло в жизни! Я живу\n",
      "на улице Ленина (это улица моего детства), а работаю учителем\n",
      "русского языка...<s> Внимание: если вы зарегистрированы или авторизованы под своим\n",
      "логином / паролем - вам необходимо войти через свою учетную\n",
      "запись Facebook для того чтобы иметь возможность оставлять комментарии к\n",
      "данной публикации..\" В рамках проекта \"Учитель года России-\n",
      "----------------------------------------\n",
      "Привет, сегодня я не могу тебя увидеть! – сказала она.\n",
      "Она взяла его за руку и потянула к себе в\n",
      "номер отеля «Беверли Хилз». Они сели на кровать друг против\n",
      "друга; он был одет так же просто как всегда –\n",
      "джинсы с футболкой да рубашка-поло без галстука… И вот они\n",
      "сидели рядом уже несколько часов подряд: смотрели телевизор или читали\n",
      "книги о путешествиях во времени — что угодно могли делать\n",
      "одновременно!.. А потом вдруг все закончилось неожиданно\n"
     ]
    }
   ],
   "source": [
    "generated = generate(model, tok, \"Привет, сегодня я\", temperature=0.0,  do_sample=False, num_beams=1, max_length=100)\n",
    "print_max_N (str (generated[0]), const_N_words_output)\n",
    "\n",
    "print (\"----------------------------------------\")\n",
    "\n",
    "generated = generate(model, tok, \"Привет, сегодня я\", temperature=1.0,  do_sample=True, num_beams=1, max_length=100)\n",
    "print_max_N (str (generated[0]), const_N_words_output)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 29,
   "metadata": {
    "colab": {
     "base_uri": "https://localhost:8080/"
    },
    "id": "X2j1L3Qk-0jk",
    "outputId": "94f824e9-67f4-4bfc-d995-e4956557d85f"
   },
   "outputs": [
    {
     "name": "stderr",
     "output_type": "stream",
     "text": [
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n",
      "Setting `pad_token_id` to `eos_token_id`:50256 for open-end generation.\n"
     ]
    },
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "Я люблю тебя, — сказал он. Она улыбнулась и поцеловала\n",
      "его в губы долгим поцелуем — так долго не целовала\n",
      "никого прежде… И вдруг она почувствовала себя такой одинокой! Она\n",
      "была одна на всем белом свете: ни друзей у нее\n",
      "нет (они все умерли), никто ее никогда больше никуда с\n",
      "собой брать даже близко к себе просто физически невозможно было\n",
      "бы; а ведь ей всего двадцать лет!.. А что будет\n",
      "потом? Что ждет их впереди?.. Как они будут жить дальше\n",
      "без друг друга?!\n",
      "----------------------------------------\n",
      "Я люблю тебя, — сказала я. Она ответила мне тем\n",
      "же: «Люблю». Я знала ее всю жизнь и не верила\n",
      "ей ни на секунду… Она была моей единственной любовью в\n",
      "жизни — любовь к человеку из плоти; это чувство было\n",
      "сильнее всего остального мира или даже Вселенной! А теперь она\n",
      "меня предала!.. Это невозможно описать словами (я просто этого никогда\n",
      "бы себе раньше представить сейчас точно сама) но все-таки можно\n",
      "выразить одним словом с большой буквы М!!! И если вы\n",
      "думаете что\n"
     ]
    }
   ],
   "source": [
    "generated = generate(model, tok, \"Я люблю\", temperature=0.0,  do_sample=False, num_beams=1, max_length=100)\n",
    "print_max_N (str (generated[0]), const_N_words_output)\n",
    "\n",
    "print (\"----------------------------------------\")\n",
    "\n",
    "generated = generate(model, tok, \"Я люблю\", temperature=1.0,  do_sample=True, num_beams=1, max_length=100)\n",
    "print_max_N (str (generated[0]), const_N_words_output)\n",
    "\n",
    "print (\"----------------------------------------\")\n",
    "\n",
    "generated = generate(model, tok, \"Я люблю\", temperature=1.0,  do_sample=True, num_beams=1, max_length=100)\n",
    "print_max_N (str (generated[0]), const_N_words_output)"
   ]
  }
 ],
 "metadata": {
  "accelerator": "GPU",
  "colab": {
   "collapsed_sections": [],
   "name": "Generate_text_with_RuGPTs_HF.ipynb",
   "provenance": []
  },
  "kernelspec": {
   "display_name": "Python 3 (ipykernel)",
   "language": "python",
   "name": "python3"
  },
  "language_info": {
   "codemirror_mode": {
    "name": "ipython",
    "version": 3
   },
   "file_extension": ".py",
   "mimetype": "text/x-python",
   "name": "python",
   "nbconvert_exporter": "python",
   "pygments_lexer": "ipython3",
   "version": "3.9.7"
  },
  "widgets": {
   "application/vnd.jupyter.widget-state+json": {
    "0342ed73509448aebad7813f3e469dde": {
     "model_module": "@jupyter-widgets/base",
     "model_module_version": "1.2.0",
     "model_name": "LayoutModel",
     "state": {
      "_model_module": "@jupyter-widgets/base",
      "_model_module_version": "1.2.0",
      "_model_name": "LayoutModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "LayoutView",
      "align_content": null,
      "align_items": null,
      "align_self": null,
      "border": null,
      "bottom": null,
      "display": null,
      "flex": null,
      "flex_flow": null,
      "grid_area": null,
      "grid_auto_columns": null,
      "grid_auto_flow": null,
      "grid_auto_rows": null,
      "grid_column": null,
      "grid_gap": null,
      "grid_row": null,
      "grid_template_areas": null,
      "grid_template_columns": null,
      "grid_template_rows": null,
      "height": null,
      "justify_content": null,
      "justify_items": null,
      "left": null,
      "margin": null,
      "max_height": null,
      "max_width": null,
      "min_height": null,
      "min_width": null,
      "object_fit": null,
      "object_position": null,
      "order": null,
      "overflow": null,
      "overflow_x": null,
      "overflow_y": null,
      "padding": null,
      "right": null,
      "top": null,
      "visibility": null,
      "width": null
     }
    },
    "04123a9b565045ee8fdc0bc27c79209b": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "HTMLModel",
     "state": {
      "_dom_classes": [],
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "HTMLModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/controls",
      "_view_module_version": "1.5.0",
      "_view_name": "HTMLView",
      "description": "",
      "description_tooltip": null,
      "layout": "IPY_MODEL_2e4e9c3b292b4fe79c78d548fcc4881e",
      "placeholder": "​",
      "style": "IPY_MODEL_18ab43ff69614fbfbcaea240758f7b73",
      "value": " 609/609 [00:00&lt;00:00, 14.2kB/s]"
     }
    },
    "11a66ff6a7874cab9deb92bf7c72aaba": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "DescriptionStyleModel",
     "state": {
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "DescriptionStyleModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "StyleView",
      "description_width": ""
     }
    },
    "18627f53dcde4f9cacfe72b50c34f097": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "DescriptionStyleModel",
     "state": {
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "DescriptionStyleModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "StyleView",
      "description_width": ""
     }
    },
    "18ab43ff69614fbfbcaea240758f7b73": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "DescriptionStyleModel",
     "state": {
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "DescriptionStyleModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "StyleView",
      "description_width": ""
     }
    },
    "1a43982afe134b62a0a845730783f7fc": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "ProgressStyleModel",
     "state": {
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "ProgressStyleModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "StyleView",
      "bar_color": null,
      "description_width": ""
     }
    },
    "1a4c204924f44c3fbbe8ef3304b95bdd": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "HTMLModel",
     "state": {
      "_dom_classes": [],
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "HTMLModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/controls",
      "_view_module_version": "1.5.0",
      "_view_name": "HTMLView",
      "description": "",
      "description_tooltip": null,
      "layout": "IPY_MODEL_e9b805e45910438ba3e8654595a5dd81",
      "placeholder": "​",
      "style": "IPY_MODEL_6825c7fb0bff41e38b0bb961d9d3da5b",
      "value": " 1.27M/1.27M [00:00&lt;00:00, 1.22MB/s]"
     }
    },
    "2e4e9c3b292b4fe79c78d548fcc4881e": {
     "model_module": "@jupyter-widgets/base",
     "model_module_version": "1.2.0",
     "model_name": "LayoutModel",
     "state": {
      "_model_module": "@jupyter-widgets/base",
      "_model_module_version": "1.2.0",
      "_model_name": "LayoutModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "LayoutView",
      "align_content": null,
      "align_items": null,
      "align_self": null,
      "border": null,
      "bottom": null,
      "display": null,
      "flex": null,
      "flex_flow": null,
      "grid_area": null,
      "grid_auto_columns": null,
      "grid_auto_flow": null,
      "grid_auto_rows": null,
      "grid_column": null,
      "grid_gap": null,
      "grid_row": null,
      "grid_template_areas": null,
      "grid_template_columns": null,
      "grid_template_rows": null,
      "height": null,
      "justify_content": null,
      "justify_items": null,
      "left": null,
      "margin": null,
      "max_height": null,
      "max_width": null,
      "min_height": null,
      "min_width": null,
      "object_fit": null,
      "object_position": null,
      "order": null,
      "overflow": null,
      "overflow_x": null,
      "overflow_y": null,
      "padding": null,
      "right": null,
      "top": null,
      "visibility": null,
      "width": null
     }
    },
    "301869cf066f4409b60e929917c75fe9": {
     "model_module": "@jupyter-widgets/base",
     "model_module_version": "1.2.0",
     "model_name": "LayoutModel",
     "state": {
      "_model_module": "@jupyter-widgets/base",
      "_model_module_version": "1.2.0",
      "_model_name": "LayoutModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "LayoutView",
      "align_content": null,
      "align_items": null,
      "align_self": null,
      "border": null,
      "bottom": null,
      "display": null,
      "flex": null,
      "flex_flow": null,
      "grid_area": null,
      "grid_auto_columns": null,
      "grid_auto_flow": null,
      "grid_auto_rows": null,
      "grid_column": null,
      "grid_gap": null,
      "grid_row": null,
      "grid_template_areas": null,
      "grid_template_columns": null,
      "grid_template_rows": null,
      "height": null,
      "justify_content": null,
      "justify_items": null,
      "left": null,
      "margin": null,
      "max_height": null,
      "max_width": null,
      "min_height": null,
      "min_width": null,
      "object_fit": null,
      "object_position": null,
      "order": null,
      "overflow": null,
      "overflow_x": null,
      "overflow_y": null,
      "padding": null,
      "right": null,
      "top": null,
      "visibility": null,
      "width": null
     }
    },
    "32289a5790a1432a89e3bf832af5c492": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "FloatProgressModel",
     "state": {
      "_dom_classes": [],
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "FloatProgressModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/controls",
      "_view_module_version": "1.5.0",
      "_view_name": "ProgressView",
      "bar_style": "success",
      "description": "",
      "description_tooltip": null,
      "layout": "IPY_MODEL_83549350a7d14d99b9cc8851b2b99689",
      "max": 1270925,
      "min": 0,
      "orientation": "horizontal",
      "style": "IPY_MODEL_62e30f4f5faf4ec9b7ed46c80024b633",
      "value": 1270925
     }
    },
    "4a267546f0644273aa404e015595e5be": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "DescriptionStyleModel",
     "state": {
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "DescriptionStyleModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "StyleView",
      "description_width": ""
     }
    },
    "4c2f70fe0910498a91712d8cde6e1b82": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "HTMLModel",
     "state": {
      "_dom_classes": [],
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "HTMLModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/controls",
      "_view_module_version": "1.5.0",
      "_view_name": "HTMLView",
      "description": "",
      "description_tooltip": null,
      "layout": "IPY_MODEL_0342ed73509448aebad7813f3e469dde",
      "placeholder": "​",
      "style": "IPY_MODEL_4a267546f0644273aa404e015595e5be",
      "value": "Downloading: 100%"
     }
    },
    "4d1e6c04dccc4684b56bd4c6cd0bc1f3": {
     "model_module": "@jupyter-widgets/base",
     "model_module_version": "1.2.0",
     "model_name": "LayoutModel",
     "state": {
      "_model_module": "@jupyter-widgets/base",
      "_model_module_version": "1.2.0",
      "_model_name": "LayoutModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "LayoutView",
      "align_content": null,
      "align_items": null,
      "align_self": null,
      "border": null,
      "bottom": null,
      "display": null,
      "flex": null,
      "flex_flow": null,
      "grid_area": null,
      "grid_auto_columns": null,
      "grid_auto_flow": null,
      "grid_auto_rows": null,
      "grid_column": null,
      "grid_gap": null,
      "grid_row": null,
      "grid_template_areas": null,
      "grid_template_columns": null,
      "grid_template_rows": null,
      "height": null,
      "justify_content": null,
      "justify_items": null,
      "left": null,
      "margin": null,
      "max_height": null,
      "max_width": null,
      "min_height": null,
      "min_width": null,
      "object_fit": null,
      "object_position": null,
      "order": null,
      "overflow": null,
      "overflow_x": null,
      "overflow_y": null,
      "padding": null,
      "right": null,
      "top": null,
      "visibility": null,
      "width": null
     }
    },
    "5dd8ef926a5f4e68bbc3ac97d540c205": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "ProgressStyleModel",
     "state": {
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "ProgressStyleModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "StyleView",
      "bar_color": null,
      "description_width": ""
     }
    },
    "60065a2297734930b2f6b9f2d97151a3": {
     "model_module": "@jupyter-widgets/base",
     "model_module_version": "1.2.0",
     "model_name": "LayoutModel",
     "state": {
      "_model_module": "@jupyter-widgets/base",
      "_model_module_version": "1.2.0",
      "_model_name": "LayoutModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "LayoutView",
      "align_content": null,
      "align_items": null,
      "align_self": null,
      "border": null,
      "bottom": null,
      "display": null,
      "flex": null,
      "flex_flow": null,
      "grid_area": null,
      "grid_auto_columns": null,
      "grid_auto_flow": null,
      "grid_auto_rows": null,
      "grid_column": null,
      "grid_gap": null,
      "grid_row": null,
      "grid_template_areas": null,
      "grid_template_columns": null,
      "grid_template_rows": null,
      "height": null,
      "justify_content": null,
      "justify_items": null,
      "left": null,
      "margin": null,
      "max_height": null,
      "max_width": null,
      "min_height": null,
      "min_width": null,
      "object_fit": null,
      "object_position": null,
      "order": null,
      "overflow": null,
      "overflow_x": null,
      "overflow_y": null,
      "padding": null,
      "right": null,
      "top": null,
      "visibility": null,
      "width": null
     }
    },
    "62867dc6996149209794082cc0bed50b": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "HTMLModel",
     "state": {
      "_dom_classes": [],
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "HTMLModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/controls",
      "_view_module_version": "1.5.0",
      "_view_name": "HTMLView",
      "description": "",
      "description_tooltip": null,
      "layout": "IPY_MODEL_f859bb420c7240faa9dea0d510139a78",
      "placeholder": "​",
      "style": "IPY_MODEL_11a66ff6a7874cab9deb92bf7c72aaba",
      "value": "Downloading: 100%"
     }
    },
    "62e30f4f5faf4ec9b7ed46c80024b633": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "ProgressStyleModel",
     "state": {
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "ProgressStyleModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "StyleView",
      "bar_color": null,
      "description_width": ""
     }
    },
    "653f7862963045b9b3a8d92c4a18c9f2": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "DescriptionStyleModel",
     "state": {
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "DescriptionStyleModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "StyleView",
      "description_width": ""
     }
    },
    "6825c7fb0bff41e38b0bb961d9d3da5b": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "DescriptionStyleModel",
     "state": {
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "DescriptionStyleModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "StyleView",
      "description_width": ""
     }
    },
    "6db2515d6c6744b89e7c619408014765": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "HTMLModel",
     "state": {
      "_dom_classes": [],
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "HTMLModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/controls",
      "_view_module_version": "1.5.0",
      "_view_name": "HTMLView",
      "description": "",
      "description_tooltip": null,
      "layout": "IPY_MODEL_913763591b6044bd906a07512773afcf",
      "placeholder": "​",
      "style": "IPY_MODEL_c5a2f496b0f44aafadb7baa4d4083ae7",
      "value": "Downloading: 100%"
     }
    },
    "83549350a7d14d99b9cc8851b2b99689": {
     "model_module": "@jupyter-widgets/base",
     "model_module_version": "1.2.0",
     "model_name": "LayoutModel",
     "state": {
      "_model_module": "@jupyter-widgets/base",
      "_model_module_version": "1.2.0",
      "_model_name": "LayoutModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "LayoutView",
      "align_content": null,
      "align_items": null,
      "align_self": null,
      "border": null,
      "bottom": null,
      "display": null,
      "flex": null,
      "flex_flow": null,
      "grid_area": null,
      "grid_auto_columns": null,
      "grid_auto_flow": null,
      "grid_auto_rows": null,
      "grid_column": null,
      "grid_gap": null,
      "grid_row": null,
      "grid_template_areas": null,
      "grid_template_columns": null,
      "grid_template_rows": null,
      "height": null,
      "justify_content": null,
      "justify_items": null,
      "left": null,
      "margin": null,
      "max_height": null,
      "max_width": null,
      "min_height": null,
      "min_width": null,
      "object_fit": null,
      "object_position": null,
      "order": null,
      "overflow": null,
      "overflow_x": null,
      "overflow_y": null,
      "padding": null,
      "right": null,
      "top": null,
      "visibility": null,
      "width": null
     }
    },
    "8fbf1c775a9b4aa8993f5c0a3627e1bc": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "HBoxModel",
     "state": {
      "_dom_classes": [],
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "HBoxModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/controls",
      "_view_module_version": "1.5.0",
      "_view_name": "HBoxView",
      "box_style": "",
      "children": [
       "IPY_MODEL_62867dc6996149209794082cc0bed50b",
       "IPY_MODEL_d3519e1b20364cf7b828eff475bd6c59",
       "IPY_MODEL_a46343cad9674916937155ed36055042"
      ],
      "layout": "IPY_MODEL_9b3a63dc84b54175a10282ac19bec625"
     }
    },
    "913763591b6044bd906a07512773afcf": {
     "model_module": "@jupyter-widgets/base",
     "model_module_version": "1.2.0",
     "model_name": "LayoutModel",
     "state": {
      "_model_module": "@jupyter-widgets/base",
      "_model_module_version": "1.2.0",
      "_model_name": "LayoutModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "LayoutView",
      "align_content": null,
      "align_items": null,
      "align_self": null,
      "border": null,
      "bottom": null,
      "display": null,
      "flex": null,
      "flex_flow": null,
      "grid_area": null,
      "grid_auto_columns": null,
      "grid_auto_flow": null,
      "grid_auto_rows": null,
      "grid_column": null,
      "grid_gap": null,
      "grid_row": null,
      "grid_template_areas": null,
      "grid_template_columns": null,
      "grid_template_rows": null,
      "height": null,
      "justify_content": null,
      "justify_items": null,
      "left": null,
      "margin": null,
      "max_height": null,
      "max_width": null,
      "min_height": null,
      "min_width": null,
      "object_fit": null,
      "object_position": null,
      "order": null,
      "overflow": null,
      "overflow_x": null,
      "overflow_y": null,
      "padding": null,
      "right": null,
      "top": null,
      "visibility": null,
      "width": null
     }
    },
    "96b56cc2217e4b27b06dfc779125bb87": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "HTMLModel",
     "state": {
      "_dom_classes": [],
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "HTMLModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/controls",
      "_view_module_version": "1.5.0",
      "_view_name": "HTMLView",
      "description": "",
      "description_tooltip": null,
      "layout": "IPY_MODEL_a805d47bc0a14af28d1b112a959553af",
      "placeholder": "​",
      "style": "IPY_MODEL_9b2794a4231b4a6d92f6f2d1dd30a3ce",
      "value": " 3.14G/3.14G [01:49&lt;00:00, 31.1MB/s]"
     }
    },
    "9b2794a4231b4a6d92f6f2d1dd30a3ce": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "DescriptionStyleModel",
     "state": {
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "DescriptionStyleModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "StyleView",
      "description_width": ""
     }
    },
    "9b3a63dc84b54175a10282ac19bec625": {
     "model_module": "@jupyter-widgets/base",
     "model_module_version": "1.2.0",
     "model_name": "LayoutModel",
     "state": {
      "_model_module": "@jupyter-widgets/base",
      "_model_module_version": "1.2.0",
      "_model_name": "LayoutModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "LayoutView",
      "align_content": null,
      "align_items": null,
      "align_self": null,
      "border": null,
      "bottom": null,
      "display": null,
      "flex": null,
      "flex_flow": null,
      "grid_area": null,
      "grid_auto_columns": null,
      "grid_auto_flow": null,
      "grid_auto_rows": null,
      "grid_column": null,
      "grid_gap": null,
      "grid_row": null,
      "grid_template_areas": null,
      "grid_template_columns": null,
      "grid_template_rows": null,
      "height": null,
      "justify_content": null,
      "justify_items": null,
      "left": null,
      "margin": null,
      "max_height": null,
      "max_width": null,
      "min_height": null,
      "min_width": null,
      "object_fit": null,
      "object_position": null,
      "order": null,
      "overflow": null,
      "overflow_x": null,
      "overflow_y": null,
      "padding": null,
      "right": null,
      "top": null,
      "visibility": null,
      "width": null
     }
    },
    "a40d9e566c344428a77b8752f6deaaa7": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "ProgressStyleModel",
     "state": {
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "ProgressStyleModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "StyleView",
      "bar_color": null,
      "description_width": ""
     }
    },
    "a46343cad9674916937155ed36055042": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "HTMLModel",
     "state": {
      "_dom_classes": [],
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "HTMLModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/controls",
      "_view_module_version": "1.5.0",
      "_view_name": "HTMLView",
      "description": "",
      "description_tooltip": null,
      "layout": "IPY_MODEL_d96db4be3488403f8c976124fd6f506b",
      "placeholder": "​",
      "style": "IPY_MODEL_18627f53dcde4f9cacfe72b50c34f097",
      "value": " 1.71M/1.71M [00:00&lt;00:00, 8.05MB/s]"
     }
    },
    "a805d47bc0a14af28d1b112a959553af": {
     "model_module": "@jupyter-widgets/base",
     "model_module_version": "1.2.0",
     "model_name": "LayoutModel",
     "state": {
      "_model_module": "@jupyter-widgets/base",
      "_model_module_version": "1.2.0",
      "_model_name": "LayoutModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "LayoutView",
      "align_content": null,
      "align_items": null,
      "align_self": null,
      "border": null,
      "bottom": null,
      "display": null,
      "flex": null,
      "flex_flow": null,
      "grid_area": null,
      "grid_auto_columns": null,
      "grid_auto_flow": null,
      "grid_auto_rows": null,
      "grid_column": null,
      "grid_gap": null,
      "grid_row": null,
      "grid_template_areas": null,
      "grid_template_columns": null,
      "grid_template_rows": null,
      "height": null,
      "justify_content": null,
      "justify_items": null,
      "left": null,
      "margin": null,
      "max_height": null,
      "max_width": null,
      "min_height": null,
      "min_width": null,
      "object_fit": null,
      "object_position": null,
      "order": null,
      "overflow": null,
      "overflow_x": null,
      "overflow_y": null,
      "padding": null,
      "right": null,
      "top": null,
      "visibility": null,
      "width": null
     }
    },
    "b824d0fa3d4749dab43ebf1d589d4b00": {
     "model_module": "@jupyter-widgets/base",
     "model_module_version": "1.2.0",
     "model_name": "LayoutModel",
     "state": {
      "_model_module": "@jupyter-widgets/base",
      "_model_module_version": "1.2.0",
      "_model_name": "LayoutModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "LayoutView",
      "align_content": null,
      "align_items": null,
      "align_self": null,
      "border": null,
      "bottom": null,
      "display": null,
      "flex": null,
      "flex_flow": null,
      "grid_area": null,
      "grid_auto_columns": null,
      "grid_auto_flow": null,
      "grid_auto_rows": null,
      "grid_column": null,
      "grid_gap": null,
      "grid_row": null,
      "grid_template_areas": null,
      "grid_template_columns": null,
      "grid_template_rows": null,
      "height": null,
      "justify_content": null,
      "justify_items": null,
      "left": null,
      "margin": null,
      "max_height": null,
      "max_width": null,
      "min_height": null,
      "min_width": null,
      "object_fit": null,
      "object_position": null,
      "order": null,
      "overflow": null,
      "overflow_x": null,
      "overflow_y": null,
      "padding": null,
      "right": null,
      "top": null,
      "visibility": null,
      "width": null
     }
    },
    "c440e8a5399a4ba9aebcc86d973134d3": {
     "model_module": "@jupyter-widgets/base",
     "model_module_version": "1.2.0",
     "model_name": "LayoutModel",
     "state": {
      "_model_module": "@jupyter-widgets/base",
      "_model_module_version": "1.2.0",
      "_model_name": "LayoutModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "LayoutView",
      "align_content": null,
      "align_items": null,
      "align_self": null,
      "border": null,
      "bottom": null,
      "display": null,
      "flex": null,
      "flex_flow": null,
      "grid_area": null,
      "grid_auto_columns": null,
      "grid_auto_flow": null,
      "grid_auto_rows": null,
      "grid_column": null,
      "grid_gap": null,
      "grid_row": null,
      "grid_template_areas": null,
      "grid_template_columns": null,
      "grid_template_rows": null,
      "height": null,
      "justify_content": null,
      "justify_items": null,
      "left": null,
      "margin": null,
      "max_height": null,
      "max_width": null,
      "min_height": null,
      "min_width": null,
      "object_fit": null,
      "object_position": null,
      "order": null,
      "overflow": null,
      "overflow_x": null,
      "overflow_y": null,
      "padding": null,
      "right": null,
      "top": null,
      "visibility": null,
      "width": null
     }
    },
    "c5a2f496b0f44aafadb7baa4d4083ae7": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "DescriptionStyleModel",
     "state": {
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "DescriptionStyleModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "StyleView",
      "description_width": ""
     }
    },
    "caa2de90473c4be698a990fde36fba2c": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "HTMLModel",
     "state": {
      "_dom_classes": [],
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "HTMLModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/controls",
      "_view_module_version": "1.5.0",
      "_view_name": "HTMLView",
      "description": "",
      "description_tooltip": null,
      "layout": "IPY_MODEL_c440e8a5399a4ba9aebcc86d973134d3",
      "placeholder": "​",
      "style": "IPY_MODEL_653f7862963045b9b3a8d92c4a18c9f2",
      "value": "Downloading: 100%"
     }
    },
    "d2460e65887645de81a494e012d96663": {
     "model_module": "@jupyter-widgets/base",
     "model_module_version": "1.2.0",
     "model_name": "LayoutModel",
     "state": {
      "_model_module": "@jupyter-widgets/base",
      "_model_module_version": "1.2.0",
      "_model_name": "LayoutModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "LayoutView",
      "align_content": null,
      "align_items": null,
      "align_self": null,
      "border": null,
      "bottom": null,
      "display": null,
      "flex": null,
      "flex_flow": null,
      "grid_area": null,
      "grid_auto_columns": null,
      "grid_auto_flow": null,
      "grid_auto_rows": null,
      "grid_column": null,
      "grid_gap": null,
      "grid_row": null,
      "grid_template_areas": null,
      "grid_template_columns": null,
      "grid_template_rows": null,
      "height": null,
      "justify_content": null,
      "justify_items": null,
      "left": null,
      "margin": null,
      "max_height": null,
      "max_width": null,
      "min_height": null,
      "min_width": null,
      "object_fit": null,
      "object_position": null,
      "order": null,
      "overflow": null,
      "overflow_x": null,
      "overflow_y": null,
      "padding": null,
      "right": null,
      "top": null,
      "visibility": null,
      "width": null
     }
    },
    "d28f7d7e63b748bf90bd92a3dc15da98": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "FloatProgressModel",
     "state": {
      "_dom_classes": [],
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "FloatProgressModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/controls",
      "_view_module_version": "1.5.0",
      "_view_name": "ProgressView",
      "bar_style": "success",
      "description": "",
      "description_tooltip": null,
      "layout": "IPY_MODEL_d2460e65887645de81a494e012d96663",
      "max": 3141928084,
      "min": 0,
      "orientation": "horizontal",
      "style": "IPY_MODEL_a40d9e566c344428a77b8752f6deaaa7",
      "value": 3141928084
     }
    },
    "d2e5151870b24a7db88a7bef126a66f3": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "HBoxModel",
     "state": {
      "_dom_classes": [],
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "HBoxModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/controls",
      "_view_module_version": "1.5.0",
      "_view_name": "HBoxView",
      "box_style": "",
      "children": [
       "IPY_MODEL_6db2515d6c6744b89e7c619408014765",
       "IPY_MODEL_d28f7d7e63b748bf90bd92a3dc15da98",
       "IPY_MODEL_96b56cc2217e4b27b06dfc779125bb87"
      ],
      "layout": "IPY_MODEL_b824d0fa3d4749dab43ebf1d589d4b00"
     }
    },
    "d3519e1b20364cf7b828eff475bd6c59": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "FloatProgressModel",
     "state": {
      "_dom_classes": [],
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "FloatProgressModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/controls",
      "_view_module_version": "1.5.0",
      "_view_name": "ProgressView",
      "bar_style": "success",
      "description": "",
      "description_tooltip": null,
      "layout": "IPY_MODEL_d7c727cbe2484175a457c8b82f5da0da",
      "max": 1713123,
      "min": 0,
      "orientation": "horizontal",
      "style": "IPY_MODEL_5dd8ef926a5f4e68bbc3ac97d540c205",
      "value": 1713123
     }
    },
    "d7673c60fa504d6bbc91dc4c8257eb27": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "HBoxModel",
     "state": {
      "_dom_classes": [],
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "HBoxModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/controls",
      "_view_module_version": "1.5.0",
      "_view_name": "HBoxView",
      "box_style": "",
      "children": [
       "IPY_MODEL_caa2de90473c4be698a990fde36fba2c",
       "IPY_MODEL_ebfa200d8b3a4bc28ae7ce6a4ef04925",
       "IPY_MODEL_04123a9b565045ee8fdc0bc27c79209b"
      ],
      "layout": "IPY_MODEL_301869cf066f4409b60e929917c75fe9"
     }
    },
    "d7c727cbe2484175a457c8b82f5da0da": {
     "model_module": "@jupyter-widgets/base",
     "model_module_version": "1.2.0",
     "model_name": "LayoutModel",
     "state": {
      "_model_module": "@jupyter-widgets/base",
      "_model_module_version": "1.2.0",
      "_model_name": "LayoutModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "LayoutView",
      "align_content": null,
      "align_items": null,
      "align_self": null,
      "border": null,
      "bottom": null,
      "display": null,
      "flex": null,
      "flex_flow": null,
      "grid_area": null,
      "grid_auto_columns": null,
      "grid_auto_flow": null,
      "grid_auto_rows": null,
      "grid_column": null,
      "grid_gap": null,
      "grid_row": null,
      "grid_template_areas": null,
      "grid_template_columns": null,
      "grid_template_rows": null,
      "height": null,
      "justify_content": null,
      "justify_items": null,
      "left": null,
      "margin": null,
      "max_height": null,
      "max_width": null,
      "min_height": null,
      "min_width": null,
      "object_fit": null,
      "object_position": null,
      "order": null,
      "overflow": null,
      "overflow_x": null,
      "overflow_y": null,
      "padding": null,
      "right": null,
      "top": null,
      "visibility": null,
      "width": null
     }
    },
    "d96db4be3488403f8c976124fd6f506b": {
     "model_module": "@jupyter-widgets/base",
     "model_module_version": "1.2.0",
     "model_name": "LayoutModel",
     "state": {
      "_model_module": "@jupyter-widgets/base",
      "_model_module_version": "1.2.0",
      "_model_name": "LayoutModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "LayoutView",
      "align_content": null,
      "align_items": null,
      "align_self": null,
      "border": null,
      "bottom": null,
      "display": null,
      "flex": null,
      "flex_flow": null,
      "grid_area": null,
      "grid_auto_columns": null,
      "grid_auto_flow": null,
      "grid_auto_rows": null,
      "grid_column": null,
      "grid_gap": null,
      "grid_row": null,
      "grid_template_areas": null,
      "grid_template_columns": null,
      "grid_template_rows": null,
      "height": null,
      "justify_content": null,
      "justify_items": null,
      "left": null,
      "margin": null,
      "max_height": null,
      "max_width": null,
      "min_height": null,
      "min_width": null,
      "object_fit": null,
      "object_position": null,
      "order": null,
      "overflow": null,
      "overflow_x": null,
      "overflow_y": null,
      "padding": null,
      "right": null,
      "top": null,
      "visibility": null,
      "width": null
     }
    },
    "e9b805e45910438ba3e8654595a5dd81": {
     "model_module": "@jupyter-widgets/base",
     "model_module_version": "1.2.0",
     "model_name": "LayoutModel",
     "state": {
      "_model_module": "@jupyter-widgets/base",
      "_model_module_version": "1.2.0",
      "_model_name": "LayoutModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "LayoutView",
      "align_content": null,
      "align_items": null,
      "align_self": null,
      "border": null,
      "bottom": null,
      "display": null,
      "flex": null,
      "flex_flow": null,
      "grid_area": null,
      "grid_auto_columns": null,
      "grid_auto_flow": null,
      "grid_auto_rows": null,
      "grid_column": null,
      "grid_gap": null,
      "grid_row": null,
      "grid_template_areas": null,
      "grid_template_columns": null,
      "grid_template_rows": null,
      "height": null,
      "justify_content": null,
      "justify_items": null,
      "left": null,
      "margin": null,
      "max_height": null,
      "max_width": null,
      "min_height": null,
      "min_width": null,
      "object_fit": null,
      "object_position": null,
      "order": null,
      "overflow": null,
      "overflow_x": null,
      "overflow_y": null,
      "padding": null,
      "right": null,
      "top": null,
      "visibility": null,
      "width": null
     }
    },
    "ebfa200d8b3a4bc28ae7ce6a4ef04925": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "FloatProgressModel",
     "state": {
      "_dom_classes": [],
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "FloatProgressModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/controls",
      "_view_module_version": "1.5.0",
      "_view_name": "ProgressView",
      "bar_style": "success",
      "description": "",
      "description_tooltip": null,
      "layout": "IPY_MODEL_60065a2297734930b2f6b9f2d97151a3",
      "max": 609,
      "min": 0,
      "orientation": "horizontal",
      "style": "IPY_MODEL_1a43982afe134b62a0a845730783f7fc",
      "value": 609
     }
    },
    "f859bb420c7240faa9dea0d510139a78": {
     "model_module": "@jupyter-widgets/base",
     "model_module_version": "1.2.0",
     "model_name": "LayoutModel",
     "state": {
      "_model_module": "@jupyter-widgets/base",
      "_model_module_version": "1.2.0",
      "_model_name": "LayoutModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/base",
      "_view_module_version": "1.2.0",
      "_view_name": "LayoutView",
      "align_content": null,
      "align_items": null,
      "align_self": null,
      "border": null,
      "bottom": null,
      "display": null,
      "flex": null,
      "flex_flow": null,
      "grid_area": null,
      "grid_auto_columns": null,
      "grid_auto_flow": null,
      "grid_auto_rows": null,
      "grid_column": null,
      "grid_gap": null,
      "grid_row": null,
      "grid_template_areas": null,
      "grid_template_columns": null,
      "grid_template_rows": null,
      "height": null,
      "justify_content": null,
      "justify_items": null,
      "left": null,
      "margin": null,
      "max_height": null,
      "max_width": null,
      "min_height": null,
      "min_width": null,
      "object_fit": null,
      "object_position": null,
      "order": null,
      "overflow": null,
      "overflow_x": null,
      "overflow_y": null,
      "padding": null,
      "right": null,
      "top": null,
      "visibility": null,
      "width": null
     }
    },
    "f9695c02534649cc870a9099f45dd24f": {
     "model_module": "@jupyter-widgets/controls",
     "model_module_version": "1.5.0",
     "model_name": "HBoxModel",
     "state": {
      "_dom_classes": [],
      "_model_module": "@jupyter-widgets/controls",
      "_model_module_version": "1.5.0",
      "_model_name": "HBoxModel",
      "_view_count": null,
      "_view_module": "@jupyter-widgets/controls",
      "_view_module_version": "1.5.0",
      "_view_name": "HBoxView",
      "box_style": "",
      "children": [
       "IPY_MODEL_4c2f70fe0910498a91712d8cde6e1b82",
       "IPY_MODEL_32289a5790a1432a89e3bf832af5c492",
       "IPY_MODEL_1a4c204924f44c3fbbe8ef3304b95bdd"
      ],
      "layout": "IPY_MODEL_4d1e6c04dccc4684b56bd4c6cd0bc1f3"
     }
    }
   }
  }
 },
 "nbformat": 4,
 "nbformat_minor": 1
}
