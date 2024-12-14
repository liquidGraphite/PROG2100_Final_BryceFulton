#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <limits>

struct Student {
    std::string lastName;
    std::string firstName;
    std::string studentNumber;
    double midterm1;
    double midterm2;
    double finalExam;
};

// Default weight percentages
double wMidterm1 = 0.25;
double wMidterm2 = 0.25;
double wFinal    = 0.50;

void printMenu();
void addStudent(std::vector<Student> &students);
void printAllStudents(const std::vector<Student> &students);
void printAllStudentsSorted(std::vector<Student> students); // pass by value to avoid changing original
void setWeighting();
void printSpecificStudent(const std::vector<Student> &students);
void removeStudent(std::vector<Student> &students);
void clearAllStudents(std::vector<Student> &students);
char calculateLetterGrade(const Student &s);
double calculateAverage(const Student &s);
int findStudentIndex(const std::vector<Student> &students, const std::string &studentNumber);

int main() {
    std::vector<Student> students;
    bool done = false;

    while(!done) {
        printMenu();
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear input buffer

        switch(choice) {
            case 1: // Print all students
                printAllStudents(students);
                break;
            case 2: // Print sorted by last name, then first name
                printAllStudentsSorted(students);
                break;
            case 3: // Add a student
                addStudent(students);
                break;
            case 4: // Calculate letter grades & show weighting info
            {
                // Just show a message that letter grades are always calculated on-the-fly
                std::cout << "Letter grades are always calculated based on current weightings:\n";
                std::cout << "Midterm1: " << wMidterm1 * 100 << "%, Midterm2: " << wMidterm2 * 100
                          << "%, Final: " << wFinal * 100 << "%\n";
                std::cout << "Use the \"Set Grade Weighting\" option (option 9) to adjust.\n";
                break;
            }
            case 5: // Print specific student data
                printSpecificStudent(students);
                break;
            case 6: // Remove a particular student
                removeStudent(students);
                break;
            case 7: // Delete all student data
                clearAllStudents(students);
                break;
            case 8: // Exit
                done = true;
                break;
            case 9: // Set weighting
                setWeighting();
                break;
            default:
                std::cout << "Invalid choice. Please choose again.\n";
        }
    }

    std::cout << "Exiting the program. Goodbye!\n";
    return 0;
}

void printMenu() {
    std::cout << "\n=== Student Information System Menu ===\n"
              << "1. Print all students\n"
              << "2. Print all students (sorted by last name, then first name)\n"
              << "3. Add a student\n"
              << "4. Display letter grade calculation info (and current weighting)\n"
              << "5. Print data of a specific student\n"
              << "6. Remove a particular student\n"
              << "7. Delete all student data\n"
              << "8. Exit\n"
              << "9. Set Grade Weighting\n"
              << "Select an option: ";
}

void addStudent(std::vector<Student> &students) {
    Student s;
    std::cout << "Enter student's first name: ";
    std::cin >> s.firstName;
    std::cout << "Enter student's last name: ";
    std::cin >> s.lastName;
    std::cout << "Enter student's number: ";
    std::cin >> s.studentNumber;
    std::cout << "Enter midterm #1 grade (0-100): ";
    std::cin >> s.midterm1;
    std::cout << "Enter midterm #2 grade (0-100): ";
    std::cin >> s.midterm2;
    std::cout << "Enter final exam grade (0-100): ";
    std::cin >> s.finalExam;

    students.push_back(s);
    std::cout << "Student added successfully!\n";
}

void printAllStudents(const std::vector<Student> &students) {
    if (students.empty()) {
        std::cout << "No students to display.\n";
        return;
    }

    std::cout << "List of all students:\n";
    for (const auto &s : students) {
        double avg = calculateAverage(s);
        char letter = calculateLetterGrade(s);
        std::cout << s.lastName << ", " << s.firstName
                  << " (" << s.studentNumber << ") - Midterm1: " << s.midterm1
                  << ", Midterm2: " << s.midterm2 << ", Final: " << s.finalExam
                  << ", Avg: " << std::fixed << std::setprecision(2) << avg << "%, Letter: " << letter << "\n";
    }
}

void printAllStudentsSorted(std::vector<Student> students) {
    if (students.empty()) {
        std::cout << "No students to display.\n";
        return;
    }

    // Sort by lastName, then by firstName
    std::sort(students.begin(), students.end(), [](const Student &a, const Student &b) {
        if (a.lastName == b.lastName)
            return a.firstName < b.firstName;
        return a.lastName < b.lastName;
    });

    std::cout << "Students sorted by last name, then first name:\n";
    for (const auto &s : students) {
        double avg = calculateAverage(s);
        char letter = calculateLetterGrade(s);
        std::cout << s.lastName << ", " << s.firstName
                  << " (" << s.studentNumber << ") - Midterm1: " << s.midterm1
                  << ", Midterm2: " << s.midterm2 << ", Final: " << s.finalExam
                  << ", Avg: " << std::fixed << std::setprecision(2) << avg << "%, Letter: " << letter << "\n";
    }
}

void setWeighting() {
    std::cout << "Current Weighting:\n"
              << "Midterm1: " << wMidterm1 * 100 << "%\n"
              << "Midterm2: " << wMidterm2 * 100 << "%\n"
              << "Final: " << wFinal * 100 << "%\n";

    std::cout << "Enter new percentage for Midterm #1 (0-100): ";
    double wm1;
    std::cin >> wm1;

    std::cout << "Enter new percentage for Midterm #2 (0-100): ";
    double wm2;
    std::cin >> wm2;

    std::cout << "Enter new percentage for Final (0-100): ";
    double wf;
    std::cin >> wf;

    double total = wm1 + wm2 + wf;
    if (total == 0) {
        std::cout << "Invalid weighting. Total should not be 0.\n";
        return;
    }

    // Normalize to sum to 1.0
    wMidterm1 = wm1 / total;
    wMidterm2 = wm2 / total;
    wFinal = wf / total;

    std::cout << "New Weighting Set:\n"
              << "Midterm1: " << wMidterm1 * 100 << "%\n"
              << "Midterm2: " << wMidterm2 * 100 << "%\n"
              << "Final: " << wFinal * 100 << "%\n";
}

void printSpecificStudent(const std::vector<Student> &students) {
    if (students.empty()) {
        std::cout << "No students in the list.\n";
        return;
    }
    std::cout << "Enter the student number to search for: ";
    std::string sNum;
    std::cin >> sNum;

    int index = findStudentIndex(students, sNum);
    if (index == -1) {
        std::cout << "Student not found.\n";
        return;
    }

    const Student &s = students[index];
    double avg = calculateAverage(s);
    char letter = calculateLetterGrade(s);
    std::cout << s.lastName << ", " << s.firstName
              << " (" << s.studentNumber << ") - Midterm1: " << s.midterm1
              << ", Midterm2: " << s.midterm2 << ", Final: " << s.finalExam
              << ", Avg: " << std::fixed << std::setprecision(2) << avg << "%, Letter: " << letter << "\n";
}

void removeStudent(std::vector<Student> &students) {
    if (students.empty()) {
        std::cout << "No students to remove.\n";
        return;
    }

    std::cout << "Enter the student number of the student to remove: ";
    std::string sNum;
    std::cin >> sNum;

    int index = findStudentIndex(students, sNum);
    if (index == -1) {
        std::cout << "Student not found.\n";
        return;
    }

    students.erase(students.begin() + index);
    std::cout << "Student removed successfully.\n";
}

void clearAllStudents(std::vector<Student> &students) {
    students.clear();
    std::cout << "All student data has been cleared.\n";
}

char calculateLetterGrade(const Student &s) {
    double avg = calculateAverage(s);
    if (avg > 90) return 'A';
    else if (avg > 80) return 'B';
    else if (avg > 70) return 'C';
    else if (avg > 60) return 'D';
    else return 'F';
}

double calculateAverage(const Student &s) {
    return s.midterm1 * wMidterm1 * 1.0
           + s.midterm2 * wMidterm2 * 1.0
           + s.finalExam * wFinal * 1.0;
}

int findStudentIndex(const std::vector<Student> &students, const std::string &studentNumber) {
    for (size_t i = 0; i < students.size(); ++i) {
        if (students[i].studentNumber == studentNumber) {
            return static_cast<int>(i);
        }
    }
    return -1; // not found
}


