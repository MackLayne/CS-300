//==============================================================
// ABCU Course Planner (Project Two)
// Author: Mackenzie Packer
// Version: 1.0
// Purpose:
//  - Load courses from CSV file
//  - Store in Binary Search Tree
//  - Print sorted course list
//  - Search and display course info
//==============================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

//==============================================================
// Course structure holds all course data
//==============================================================
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

//==============================================================
// BST Node structure
// Each node stores one Course object
//==============================================================
struct Node {

    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // constructor with course data
    Node(Course course) {
        this->course = course;
        left = nullptr;
        right = nullptr;
    }
};

//==============================================================
// Binary Search Tree class
//==============================================================
class BinarySearchTree {
private:
    Node* root;

    // recursive helpers for insert
    void insertNode(Node* node, Course course);
    
    // recursive helpers for print course list
    void inOrder(Node* node);

public:
    BinarySearchTree();
    void Insert(Course course);
    Course Search(string courseNumber);
    void PrintCourse(string courseNumber);
    void PrintCourseList();
};

//==============================================================
// Constructor initializes empty tree
//==============================================================
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

//==============================================================
// Insert wrapper handles first node case
//==============================================================
void BinarySearchTree::Insert(Course course) {
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        insertNode(root, course);
    }
}

//==============================================================
// Recursive insert into BST
// Orders by courseNumber alphabetically
//==============================================================
void BinarySearchTree::insertNode(Node* node, Course course) {
    if (course.courseNumber < node->course.courseNumber) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            insertNode(node->left, course);
        }
    }
    else {
        // course number is greater than or equal to current node
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            insertNode(node->right, course);
        }
    }
}

//==============================================================
// Search for a course by course number
//==============================================================
Course BinarySearchTree::Search(string courseNumber) {
    Node* current = root;

    while (current != nullptr) {
        if (current->course.courseNumber == courseNumber) {
            return current->course;
        }
        // search left sub tree
        else if (courseNumber < current->course.courseNumber) {
            current = current->left;
        }
        // search right sub tree
        else {
            current = current->right;
        }
    }

    // return empty course if not found
    return Course();
}

//==============================================================
// Print single course + prerequisites
//==============================================================
void BinarySearchTree::PrintCourse(string courseNumber) {
    Course course = Search(courseNumber);

    if (course.courseNumber.empty()) {
        cout << "Course not found." << endl;
        return;
    }

    // if found print course number and title
    cout << course.courseNumber << ", " << course.courseTitle << endl;
    cout << "Prerequisites: ";

    // no prerequisites
    if (course.prerequisites.empty()) {
        cout << "None";
    }
    // print prerequisites
    else {
        for (size_t i = 0; i < course.prerequisites.size(); i++) {
            cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
    }

    cout << endl;
}

//==============================================================
// In-order traversal prints sorted course list
//==============================================================
void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->course.courseNumber
             << ", "
             << node->course.courseTitle
             << endl;
        inOrder(node->right);
    }
}

//==============================================================
// Print full course list in sorted order
//==============================================================
void BinarySearchTree::PrintCourseList() {
    cout << "Here is a sample schedule:" << endl;
    inOrder(root);
}

//==============================================================
// Load courses from CSV file into BST
//==============================================================
void LoadCourses(string fileName, BinarySearchTree* bst) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: File could not be opened." << endl;
        return;
    }

    vector<string> allCourseNumbers; // catalog of every course number in the file
    string line;

    // Pass 1: collect all course numbers ---
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            // Remove Windows carriage return character
            token.erase(remove(token.begin(), token.end(), '\r'), token.end());
            tokens.push_back(token);
        }

        if (tokens.size() < 2) continue;

        allCourseNumbers.push_back(tokens[0]);
    }

    // Reset the file stream back to the beginning for the second pass
    file.clear();
    file.seekg(0);

    // Pass 2: build Course objects and insert into BST
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            token.erase(remove(token.begin(), token.end(), '\r'), token.end());
            tokens.push_back(token);
        }

        if (tokens.size() < 2) continue;

        Course course;
        course.courseNumber = tokens[0];
        course.courseTitle  = tokens[1];

        // Any tokens beyond index 1 are prerequisite course numbers
        for (size_t i = 2; i < tokens.size(); i++) {
            course.prerequisites.push_back(tokens[i]);
        }

        bst->Insert(course);
    }

    cout << "Courses loaded successfully." << endl;

    file.close();
}


// ============================================================
// MAIN
// Drives the interactive menu loop. The user can:
//   1 — Load courses from a CSV file into the BST
//   2 — Print all courses in sorted order
//   3 — Look up and print a single course
//   9 — Exit the program
// ============================================================

int main() {
    BinarySearchTree bst;

    bool loaded  = false; // tracks whether course data has been loaded
    int  choice  = 0;
    string fileName;
    string userInput;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        // Display the menu each iteration
        cout << "\n1. Load Data Structure.\n"
             << "2. Print Course List.\n"
             << "3. Print Course.\n"
             << "9. Exit\n"
             << "What would you like to do? ";

        cin >> choice;

        if (choice == 1) {
            // Ask for the file name and load courses into the BST
            cout << "Enter file name: ";
            cin >> fileName;
            LoadCourses(fileName, &bst);
            loaded = true;

        } else if (choice == 2) {
            // List all courses  requires data to be loaded first
            if (!loaded) {
                cout << "Please load data first." << endl;
            } else {
                bst.PrintCourseList();
            }

        } else if (choice == 3) {
            // Look up a specific course requires data to be loaded first
            if (!loaded) {
                cout << "Please load data first." << endl;
            } else {
                cout << "What course do you want to know about? ";
                cin >> userInput;

                // Normalize input to uppercase so lookup is case-insensitive
                transform(userInput.begin(), userInput.end(), userInput.begin(), ::toupper);

                bst.PrintCourse(userInput);
            }

        } else if (choice == 9) {
            cout << "Thank you for using the course planner!" << endl;

        } else {
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}
