for FILE in *-help.pd; do cat $FILE | sed  '1 s/^\(#N canvas\) \([1-9][0-9]* [1-9][0-9]*\)/\1 183 83/' > ../newhelp/$FILE; done
