SRC = src/vm.c
INC = -I src

TEST_SRC =  unittest/framework/TestMain.c
TEST_SRC += unittest/tests/vm_test.c

TEST_INC = -I unittest/framework

test : ${SRC} ${TEST_SRC}
	mkdir -p out
	gcc ${SRC} ${TEST_SRC} ${INC} ${TEST_INC} -o out/test
	out/test