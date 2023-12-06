from backend_tests import test_expr_valid, test_valid, test_expr
from test_runtime import ErrorType, print_exit, test, nr_failed


def test_invalid_token(code):
    test(code, "", expected_return_code=ErrorType.error_lexer)


def test_all():
    test_invalid_token("""
    1.2.3
    """)

    test_expr("""\"""
\"""
    """, "")

    test_expr("""
\"""
\"""
    """, "")

    test_expr("""\"""

\"""
    """, "")

    test_expr("""\"""
abc
\"""
    """, "abc")

    test_expr("""\"""
    abc
\"""
    """, "    abc")

    test_expr("""\"""
    abc
    \"""
    """, "abc")

    test_invalid_token("""\"""
    abc
    abc\"""
    """)

    test_invalid_token("""\"""abc
    \"""
    """)

    test_invalid_token("""\"""
   abc
    \"""
    """)

    test_invalid_token("""\"""
    abc
abc
    \"""
    """)

# this is confusing as hell, but there are only *two* newlines at the blank
# line, and that is illegal acc to swift compiler (really, whoever invented this,
# deserves to be hung by his balls)
    test_invalid_token("""\"""
  
    \"""
    """)

    test_expr("""\"""
        abc
    \"""
    """, "    abc")

    test_expr("""\"""

    \"""
    """, "")

    test_expr("""\"""


    \"""
    """, "\n")

    # this looks redundant, but i found some bugs in our compiler that only
    # showed with single character, so i wanted to be sure

    test_expr("""\"""
a
\"""
    """, "a")

    test_expr("""\"""
    a
\"""
    """, "    a")

    test_expr("""\"""
    a
    \"""
    """, "a")

    test_invalid_token("""\"""
    a
    a\"""
    """)

    test_invalid_token("""\"""a
    \"""
    """)

    test_invalid_token("""\"""
   a
    \"""
    """)

    test_invalid_token("""\"""
    a
a
    \"""
    """)

    test_expr("""\"""
        abc
    \"""
    """, "    abc")

    test_expr("""\"""

    \"""
    """, "")

    test_expr("""\"""


    \"""
    """, "\n")

    test_invalid_token("""\"""
 
    \"""
    """)

    test_expr_valid("""\"""
    \\u{aBcD234}
    \"""
    """)

    test_expr("""\"""
    \\t\\r\\\\
    \"""
    """, "\t\r\\")

    test_invalid_token("""\"""
    \\i
    \"""
    """)

    # also for normal strings

    test_expr("""
    "\\t\\r\\\\"
    """, "\t\r\\")

    test_invalid_token("""
    "\\i"
    """)


if __name__ == "__main__":
    test_all()
    print_exit()
