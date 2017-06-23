clear
echo "==================================================================" 
echo "==              B E N C H M A R K   N U M B E R S               ==" 
echo "==                                                              ==" 
echo "==              C L A N G      C O M P I L E R                  ==" 
echo "=================================================================="
echo "."
echo "C O M P I L I N G . . . . . . . . . . ."
echo "."
clang++ -O3  -std=c++11  ../file_generator.cpp -march=native -w -fexceptions  -I../../include -o file_generator
echo "."
clang++ -O3  -std=c++11 ../benchmark_numbers.cpp  -march=native -w -fexceptions  -I../../include   -I../include  -o benchmark_numbers

echo "R U N N I N G . . . . . . . . . . ."
echo " ( The time needed is around 5 to 20 minutes depending of your machine ). . . . . . ."
./file_generator input.bin 125000000
./benchmark_numbers 
rm input.bin
rm file_generator
rm benchmark_numbers

