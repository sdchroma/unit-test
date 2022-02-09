echo "Starting CI FORMAT verification script"

clang-format-12 --dry-run --Werror src/main.c 
RETVAL=$?

if [ "$RETVAL" == 1 ]; then
  echo "src/main.c format ERROR"
else
  echo "src/main.c format OK"
fi

clang-format-12 --dry-run --Werror src/test/testOsCirBuff.c
RETVAL=$?

if [ "$RETVAL" == 1 ]; then
  echo "src/test/testOsCirBuff.c format ERROR"
else
  echo "src/test/testOsCirBuff.c format OK"
fi