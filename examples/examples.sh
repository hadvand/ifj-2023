#/bin/bash

# IFJ23 - Priklady pro studenty pro dokresleni zadani (spusteni konfigurovano pro server Merlin)

COMPILER="swift "  # zde muze byt vas prekladac IFJ23 (napr. ./ifj23)
HELPER="ifj23.swift"
INTERPRETER="./run.sh"  # zde muze byt interpret IFJcode23 (./ic23int)

for i in *.swift; do

	if [[ $i == $HELPER ]]; then
		continue
	fi

	IN=`echo $i | sed 's/\(.*\)\.swift/\1.in/g'`
	OUT=`echo $i | sed 's/\(.*\)\.swift/\1.out/g'`
	PRG=$i

	# $COMPILER $i $HELPER > $PRG   # nekontroluji se chyby prekladu (pro chybove testy nutno rozsirit)
	# RETURNED_COMPILER=$?

	echo -n "DIFF: $INTERPRETER $PRG and $OUT: "
	cat $IN | $INTERPRETER $PRG  -suppress-warnings 2> >(sed $'s,.*,\e[31m&\e[m,'>&2) | diff - $OUT > /dev/null 2> /dev/null
	DIFFOK=$?
	if [ "x$DIFFOK" == "x0" ]; then
		echo "OK"
	else
		echo "DIFFERENCE to $OUT"
		cat $IN | $INTERPRETER $PRG  -suppress-warnings 2> >(sed $'s,.*,\e[31m&\e[m,'>&2) | diff - $OUT 
	fi
	echo
done;

# rm *.tmp

exit 0
