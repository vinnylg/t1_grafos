#!/usr/bin/env bash
REGEX="^[0-9]\.[0-9]{6}\.txt$"
# basePath=$(echo $BASH_SOURCE | rev | cut -c 11- | rev)
TESTS_PATH=$(pwd)/testes
FILES=$(ls "$TESTS_PATH")
EXIT_CODE=0
if [[ -n "$FILES" ]]; then
    for f in $FILES; do
        command=$(echo "$f" | grep -P -q $REGEX)
        if [[ $? -eq 0 ]]; then
             OUTPUT=$(./teste < $TESTS_PATH/$f | cut -d' ' -f8)
             filename=$(basename -- "$f")
             TMP="${filename%.*}"
             if [[ $TMP != $OUTPUT ]]; then
                    (>&2 echo "ERROR: $f should be $OUTPUT but got it $TMP")
                    EXIT_CODE=1
            else
                echo "SUCCESS: $OUTPUT = $TMP "
            fi
        fi
    done
fi

exit $EXIT_CODE
