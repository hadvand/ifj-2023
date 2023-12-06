from test_runtime import ErrorType, test, nr_failed

def test_expression_valid(code):
    code = f"""
        let result = {code}
    """
    test(code, "")

def test_expression_invalid(code):
    code = f"""
        let result = {code}
    """
    test(code, "", expected_return_code=ErrorType.error_parser)


def test_all():
    test_expression_valid("1")
    test_expression_valid("1+1")
    test_expression_valid("(1+1)")
    test_expression_valid("2*(2+3)")
    test_expression_valid("2<3")
    test_expression_valid("2*((3+4))")
    test_expression_valid("\"U\"+\"wU\"")

    test_expression_invalid("(2+2(")
    test_expression_invalid(")2+2)")
    test_expression_invalid("!2")
    test_expression_invalid("4!2")
    test_expression_invalid("1*2*")
    test_expression_invalid("1++1")
    test_expression_invalid("+")
    test_expression_invalid("(1+1")
    test_expression_invalid("2<3>4")
    test_expression_invalid("2(3+4)")



if __name__ == "__main__":
    test_all()
    exit(nr_failed)
