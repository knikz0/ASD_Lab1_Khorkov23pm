#include <iostream> 
#include <fstream>  
#include <sstream>  
#include <vector>   
#include <random>   
#include <iomanip> // std::setw
#include <utility> // std::pair

/* 
ASD_Lab1 - Implementing 8 Sorting Algorithms
Author: Khorkov Egor 23PM

Overview: 
    Create an interface for utilizing the program and the following 8 sorting algorithms: 
        "Selection Sort",
        "Bubble Sort",
        "Insertion Sort",
        "Binary Insertion Sort",
        "Shaker Sort",
        "Shell Sort",
        "Heap Sort",
        "Quick Sort"
    Display a table showing the number of comparisons and swaps performed.
    Compile the program using the command line.
*/

using namespace std;


bool isValideNumbers(vector<int> numbers, const string& str)
{
    for (int i = 1; i < numbers.size(); i++) {
        if (str == "sorted") {
            if (numbers[i - 1] > numbers[i])
                return false;
        }
        if (str == "reversedandsortered") {
            if (numbers[i - 1] < numbers[i])
                return false;
        }
    }
    return true;
}

int ValidateChoice(int choice)
{
    if (choice != 1 && choice != 2) 
    {
        throw invalid_argument("Error: Invalid input. Please enter '1' or '2'."); // Используем исключения для обработки ошибок
    }
    return choice; // Возвращаем корректный выбор
}

// Новая функция для ввода данных
int InputDataEntryRequest(const string& str)
{
    int choice_number = 0;

    while (true) {
        // Декоративное меню
        cout << "\n===========================================" << endl;
        cout << "           Data Entry Menu                " << endl;
        cout << "===========================================" << endl;
        cout << "1. Enter 200 " + str + " integer numbers (separated by spaces)." << endl;
        cout << "2. Read numbers from file: " + str + ".txt" << endl;
        cout << "------------------------------------------" << endl;
        cout << "Note: If you enter more than 200 numbers, additional numbers will be ignored." << endl;
        cout << "Please choose an option (1 or 2): ";

        cin >> choice_number;

        try {
            return ValidateChoice(choice_number); // Проверка выбора
        } catch (const invalid_argument& e) {
            cerr << "\n" << e.what() << endl; // Печать ошибки
        }

        cout << "\nPlease try again." << endl;
    }
}



vector<int> readNumbers(istream& input) {
    int number;
    vector<int> numbers;
    while (input >> number)
    {
        numbers.push_back(number);
        if (numbers.size() == 200) {
            break;
        }
    }

    int nums_size = numbers.size();
    if (nums_size < 200) {
        string str_size = to_string(nums_size);
        cout << "\nError: Need 200 numbers not " + str_size + " numbers";
        exit(1);
    }
    return numbers;
}

vector<int> getValuesFromFileOrInput(int choice_number, const string& str)
{
    vector<int> numbers;

    if (choice_number == 1)
    {
        string input;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clean buffer
        cout << "Enter int numbers through space: ";
        getline(cin, input); // full line
        stringstream ss(input);
        numbers = readNumbers(ss);
    }
    if (choice_number == 2)
    {
        ifstream file(str + ".txt"); // open file

        if (file.is_open())
        {
            cout << "\nFile is open succesfully!\n" << endl;
        }
        else
        {
            cout << "\nError file not found!" << endl;
            exit(1);
        }

        numbers = readNumbers(file);
        file.close();
    }

    if (!isValideNumbers(numbers, str))
    {
        cout << "Numbers aren't " + str;
        exit(1);
    }
    return numbers;
}

vector<int> getRandomValues()
{
    // Initialize generator of random numbers
    random_device rd;
    mt19937 gen(rd()); 
    uniform_real_distribution<> distrib(0, 300); 

    vector<int> numbers(200); 

    for (size_t i = 0; i < numbers.size(); ++i) {
        numbers[i] = distrib(gen); 
    }
    
    return numbers;
}


void printVector(vector<int> numbers)
{
    for (int i = 0; i < numbers.size(); i++)
        cout << numbers[i] << " ";
    cout << "\n";
}


pair<int, int> SelectionSort(vector<int> numbers)
{
    // Sorting by simple selection
    int nums_size = numbers.size();
    int comparisons = 0, permutations = 0;

    for (int i = 0; i < nums_size - 1; i++)
    {
        int index_max = 0;
        int swap_index = nums_size - i - 1;

        for (int j = 1; j <= swap_index; j++)
        {
            if (numbers[index_max] < numbers[j])
                index_max = j;
            comparisons++;
        }

        swap(numbers[index_max], numbers[swap_index]);
        permutations++;
    }
    return pair<int, int>(comparisons, permutations);
}


pair<int, int> BubbleSort(vector<int> numbers)
{
    // Sorting by simple exchange
    int nums_size = numbers.size();
    int comparisons = 0, permutations = 0;

    for (int i = 0; i < nums_size - 1; i++)
    {
        for (int j = 0; j < nums_size - i - 1; j++)
        {
            if (numbers[j] > numbers[j + 1])
            {
                swap(numbers[j], numbers[j + 1]);
                permutations++;
            }
            comparisons++;
        }
    }
    return pair<int, int>(comparisons, permutations);
}


pair<int, int> InsertionSort(vector<int> numbers)
{
    // Sorting by simple inserts
    int nums_size = numbers.size();
    int comparisons = 0, permutations = 0;

    for (int i = 1; i < nums_size; i++)
    {
        int key = numbers[i];
        int j;  // allocate memory in this namespace
        for (j = i - 1; j >= 0 && numbers[j] > key; j--)
        {
            comparisons++;

            numbers[j + 1] = numbers[j];
            permutations++;
        }
        // Although c++ has lazy '&&' operation
        // i'll count that comparison will do
        comparisons++;

        numbers[j + 1] = key;
        permutations++;
    }
    return pair<int, int>(comparisons, permutations);
}


pair<int, int> InsertionBinarySort(vector<int> numbers)
{
    // Sorting by simple inserts with binary search
    // Upgrade version of sorting by simple inserts
    int nums_size = numbers.size();
    int comparisons = 0, permutations = 0;

    for (int i = 1; i < nums_size; i++)
    {
        int key = numbers[i];
        int k = 0, j = i - 1;

        while (k <= j) 
        {
            int mid_index = k + (j - k) / 2;
            // move by 1 so that it is not mid_index
            if (key < numbers[mid_index])
                j = mid_index - 1; 
            else
                k = mid_index + 1; 
            comparisons++;
        }

        // move elements
        for (int m = i - 1; m >= k; m--)
        {
            numbers[m + 1] = numbers[m];
            permutations++; 
        }

        numbers[k] = key;
        permutations++; 
    }
    return pair<int, int>(comparisons, permutations);
}


pair<int, int> SheikerSort(vector<int> numbers)
{
    int nums_size = numbers.size();
    int comparisons = 0, permutations = 0;

    int left_edge = 1;
    int right_edge = nums_size - 1;
    while (left_edge <= right_edge)
    {
        for (int i = right_edge; i >= left_edge; i--)
        {
            if (numbers[i - 1] > numbers[i])
            {
                swap(numbers[i - 1], numbers[i]);
                permutations++;
            }
            comparisons++;
        }
        left_edge++;


        for (int i = left_edge; i <= right_edge; i++)
        {
            if (numbers[i - 1] > numbers[i])
            {
                swap(numbers[i - 1], numbers[i]);
                permutations++;
            }
            comparisons++;
        }
        right_edge--;
    }
    return pair<int, int>(comparisons, permutations);
}


pair<int, int> ShellSort(vector<int> numbers)
{
    int nums_size = numbers.size();
    int comparisons = 0, permutations = 0;

    int half_length = nums_size / 2;
    
    // 0 is not half length
    while (half_length)
    {
        for (int i = half_length; i < nums_size; i++)
        {
            int key = numbers[i];
            int j = i;

            for (j = j - half_length; j >= 0 && numbers[j] >= key; j -= half_length)
            {
                comparisons++;

                numbers[j + half_length] = numbers[j];
                permutations++;
            }
            comparisons++;

            numbers[j + half_length] = key;
            permutations++;
        }

        half_length /= 2;
    }
    return pair<int, int>(comparisons, permutations);
}


void sift(vector<int>& numbers, int size, int parent_index, int& comparisons, int& permutations)
{
    int left_child_index = 2 * parent_index + 1;
    int right_child_index = 2 * parent_index + 2;

    // Parent doesn't have children
    if (left_child_index > size - 1)
        return;

    int max_child;
    // If parent has child(-s). He's defenetely has left child.
    int left_child = numbers[left_child_index];

    if (right_child_index <= size - 1)
    {
        int right_child = numbers[right_child_index];
        max_child = max(left_child, right_child);
        comparisons++;
    }
    else
    {
        max_child = left_child;
    }

    int parent = numbers[parent_index];
    comparisons++;
    if (max_child > parent)
    {
        int swap_index = max_child == left_child ? left_child_index : right_child_index;
        swap(numbers[parent_index], numbers[swap_index]);
        permutations++;

        // figure out where fall down element: left or right. 
        // It'll be a parent element.
        sift(numbers, size, swap_index, comparisons, permutations);
    }
}


int getLatestParentIndex(int size)
{
    size -= 1;
    return size / 2;
}


vector<int> BuildHeap(vector<int> numbers, int size, int& comparisons, int& permutations)
{
    int parent_index = getLatestParentIndex(size) - 1;

    while (parent_index >= 0) {
        sift(numbers, size, parent_index, comparisons, permutations);
        // the next the most right parent index by count
        // Count: the most down right to left
        parent_index--;
    }
    return numbers;
}


pair<int, int> HeapSort(vector<int> numbers)
{
    int nums_size = numbers.size();
    int comparisons = 0, permutations = 0;

    numbers = BuildHeap(numbers, nums_size, comparisons, permutations);
    
    int end = nums_size - 1;
    while (end > 0) {
        swap(numbers[0], numbers[end]);
        permutations++;

        end--;
        sift(numbers, end + 1, 0, comparisons, permutations);
    }
    return pair<int, int>(comparisons, permutations);
}


pair<int, int> QuickSort(
    vector<int> numbers, int& comparisons, int& permutations, int left, int right
)
{
    int l = left, r = right;
    // average element is pivot element
    int piv = numbers[(l + r) / 2]; 

    while (l <= r)
    {
        while (numbers[l] < piv) {
            comparisons++;
            l++;
        }
        comparisons++;

        while (numbers[r] > piv) {
            comparisons++;
            r--;
        }
        comparisons++;

        if (l <= r) {
            swap(numbers[l++], numbers[r--]);
            permutations++;
        }
    }

    if (left < r) {
        QuickSort(numbers, comparisons, permutations, left, r);
    }
    if (right > l) {
        QuickSort(numbers, comparisons, permutations, l, right);
    }

    return pair<int, int>(comparisons, permutations);
}


pair<int, int> wrapper_qsort(vector<int> numbers)
{
    int comparisons = 0, permutations = 0;
    return QuickSort(numbers, comparisons, permutations, 0, numbers.size() - 1);
}


// Function to print sorting results in a simple line-by-line format
void PrintSortingResults(const vector<pair<int, int> >& comparisons_permutations, int sort_count, int number_of_vectors) {
    // Names of sorting algorithms in order of implementation
    const string sorting_algorithms[] = {
        "SelectionSort",
        "BubbleSort",
        "InsertionSort",
        "InsertionBinarySort",
        "ShakerSort",
        "ShellSort",
        "HeapSort",
        "QuickSort"
    };

    // Iterate through each sorting algorithm and print results
    for (int i = 0; i < sort_count; ++i) {
        cout << sorting_algorithms[i] << ":" << endl; // Print the algorithm name

        // Print results for ordered, reverse ordered, and random vectors
        for (int j = 0; j < number_of_vectors; ++j) {
            int index = j * sort_count + i; // Calculate index in the results array
            cout << "    Vector " << (j + 1) << " - Comparisons: " 
                 << comparisons_permutations[index].first 
                 << ", Permutations: " 
                 << comparisons_permutations[index].second << endl; // Print comparisons and permutations for each vector
        }
        cout << endl; // New line for separation between different sorting algorithms
    }

    // Footer indicating the meaning of the counts
    cout << "Counts are as follows:\nComparisons: number of element comparisons\nPermutations: number of swaps made\n" << endl;
}


int main()
{
    int choice_number;

    choice_number = InputDataEntryRequest("sorted");
    vector<int> ordered_numbers = getValuesFromFileOrInput(choice_number, "sorted");
    
    choice_number = InputDataEntryRequest("reversedandsortered");
    vector<int> reverse_ordered_numbers = getValuesFromFileOrInput(choice_number, "reversedandsortered");

    vector<int> random_numbers = getRandomValues();
    
    int sort_count = 8, number_of_vectors = 3;
    vector<pair<int, int> > comparisons_permutations(sort_count * number_of_vectors);

    // all data values
    vector<vector<int> > vectors(number_of_vectors);
    vectors[0] = ordered_numbers;
    vectors[1] = reverse_ordered_numbers;
    vectors[2] = random_numbers;
    
    for (int i = 0; i < number_of_vectors; i++) {
        vector<int> vec = vectors[i];
        
        // n-1 value is comparisons
        // n-2 value is permutations
        int i_sort_count = i * sort_count;
        comparisons_permutations[0 + i_sort_count] = SelectionSort(vec);
        comparisons_permutations[1 + i_sort_count] = BubbleSort(vec);
        comparisons_permutations[2 + i_sort_count] = InsertionSort(vec);
        comparisons_permutations[3 + i_sort_count] = InsertionBinarySort(vec);
        comparisons_permutations[4 + i_sort_count] = SheikerSort(vec);
        comparisons_permutations[5 + i_sort_count] = ShellSort(vec);
        comparisons_permutations[6 + i_sort_count] = HeapSort(vec);
        // to wrap qsort not to create optional vars
        comparisons_permutations[7 + i_sort_count] = wrapper_qsort(vec);
    }

    // print table with getting results
    PrintSortingResults(comparisons_permutations, sort_count, number_of_vectors);
}