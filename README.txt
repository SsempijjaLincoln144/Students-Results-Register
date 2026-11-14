
STUDENT RESULTS REGISTER - SCENARIO 6
Student Name: SSEMPIJJA LINCOLN
Reg Number: VU-BSF-2503-2170-DAY

--- FILES INCLUDED ---
1. DynamicArray.h              (Contains the primary data structure and StudentRecord struct)
2. StudentResultsRegister.cpp  (Contains the sorting, searching, and main() function)
3. students.txt                (The input sample data file)
4. README.txt                  (This file)
5.students_out.txt             (This contains sorted files)

--- HOW TO COMPILE ---
Open your terminal in the directory containing these files and run:
g++ -std=c++17 -g -fsanitize=address,undefined StudentResultsRegister.cpp -o register

(Note: The -fsanitize flag is included as required for the Task A3 memory leak verification).

--- HOW TO RUN ---
Ensure that "students.txt" is in the exact same folder as the compiled executable. Then run:
./register

--- EXPECTED OUTPUT ---
The program will load the data, print the sorted records to the terminal, execute the binary search tests, save the updated data to a new file named "students_out.txt", and finally run the timing experiment comparing Insertion Sort and Merge Sort.