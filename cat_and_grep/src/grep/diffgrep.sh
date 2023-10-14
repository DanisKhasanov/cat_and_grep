filename1=diffgrep.sh;
filename2=s21_grep.c;
# filename3 is also used for -f flag patterns:
filename3=test.txt;
echo ----------------------------------------------*GREP 1
diff <(grep int "$filename2" "$filename1") <(./s21_grep int "$filename2" "$filename1") -s -q
echo ----------------------------------------------*GREP 1 -flag e
diff <(grep -e int "$filename2" "$filename1") <(./s21_grep -e int "$filename2" "$filename1") -s -q
echo ----------------------------------------------*GREP 2 -flag i
diff <(grep -i int "$filename1") <(./s21_grep -i int "$filename1") -s -q
echo ----------------------------------------------*GREP 3 -flag v
diff <(grep -v int "$filename1") <(./s21_grep -v int "$filename1") -s -q
echo ----------------------------------------------*GREP 4 -flag c
diff <(grep -c int "$filename1") <(./s21_grep -c int "$filename1") -s -q
echo ----------------------------------------------*GREP 5 -flag l
diff <(grep -l int "$filename2" "$filename3" "$filename1") <(./s21_grep -l int "$filename2" "$filename3" "$filename1") -s -q
echo ----------------------------------------------*GREP 6 -flag n
diff <(grep -n int "$filename1") <(./s21_grep -n int "$filename1") -s -q
echo ----------------------------------------------*GREP 7 BONUS -flag h
diff <(grep -h int "$filename1") <(./s21_grep -h int "$filename1") -s -q 
echo ----------------------------------------------*GREP 8 BONUS -flag o
diff <(grep -o int "$filename1") <(./s21_grep -o int "$filename1") -s -q
echo ----------------------------------------------*GREP 9 BONUS -flag h and two file
diff <(grep -h int "$filename2" "$filename1") <(./s21_grep -h int "$filename2" "$filename1") -s -q
echo ----------------------------------------------*GREP 10 BONUS -nofile
diff <(grep int -s aboba) <(./s21_grep int -s aboba) -s -q
echo ----------------------------------------------*GREP 11 BONUS -flag f
diff <(grep -f "$filename3" "$filename2") <(./s21_grep -f "$filename3" "$filename2") -s -q
echo ----------------------------------------------*GREP 12 BONUS -flag in
diff <(grep -in int "$filename1") <(./s21_grep -in int "$filename1") -s -q
echo ----------------------------------------------*GREP 13 BONUS -flag cv
diff <(grep -cv int "$filename1") <(./s21_grep -cv int "$filename1") -s -q
echo ----------------------------------------------*GREP 14 BONUS - flag iv
diff <(grep -iv int "$filename1") <(./s21_grep -iv int "$filename1") -s -q
echo ----------------------------------------------*GREP 15 BONUS - flag lv
diff <(grep -lv int "$filename2" "$filename3" "$filename1") <(./s21_grep -lv int "$filename2" "$filename3" "$filename1") -s -q
echo ----------------------------------------------*GREP 16 BONUS -flag ho
diff <(grep -ho int "$filename2" "$filename1") <(./s21_grep -ho int "$filename2" "$filename1") -s -q
echo ----------------------------------------------*GREP 17 BONUS -flag nf
diff <(grep -nf "$filename3" "$filename2") <(./s21_grep -nf "$filename3" "$filename2") -s -q
echo ----------------------------------------------*Tests ended*
