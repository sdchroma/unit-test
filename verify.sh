echo "Starting CI UNIT TEST verification script"

FILE=output/main

echo "Trying to execute " $FILE

rm output.txt
$FILE >> output.txt
RETVAL=$?

if [ -f "$FILE" ]; then
  echo "Retval is 0, File Exists"
else
  echo "Retval is not 0, File Not Found, exiting..."
  exit 1
fi

OUTPUT=$(grep -c 'OK' output.txt)
echo "OUTPUT(OK) is: " $OUTPUT
echo
echo "=====UNIT TEST RESULTS====="
cat output.txt
rm output.txt
echo "==========================="
echo
echo

if [ "$OUTPUT" == 1 ]; then
  echo "Unit Tests are OK"
else
  echo "Unit Tests are FAIL"
  exit 1
fi