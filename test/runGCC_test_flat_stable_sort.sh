clear
echo "==================================================================" 
echo "==        T E S T     F L A T _ S T A B L E _ S O R T           ==" 
echo "==                                                              ==" 
echo "==                 G C C      C O M P I L E R                   ==" 
echo "=================================================================="
echo "."
echo "C O M P I L I N G . . . . . . . . . . ."
echo "."
g++ ./test_flat_stable_sort.cpp -std=c++11 -march=native -w -fexceptions -O3 -I../include  -s  -o test_flat_stable_sort

echo "R U N N I N G . . . . . . . . . . ."
echo "( The time needed is around 10 minutes depending of your machine )"
./test_flat_stable_sort

rm test_flat_stable_sort
echo "."

echo "."
echo "E N D"
echo "."
