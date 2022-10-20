//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Robin Robinson
// Version     : 1.0
//============================================================================

#include <fstream>
#include <sstream>
#include <iostream>
#include "CSVparser.hpp"
#include <vector>
using namespace csv;
using namespace std;

// Define structure with course info
struct Course {
    string courseNumber;
    string courseName;
    string coursePrereqs;
    string courseId;
};

struct Node {
    Course course;
    Node* left;
    Node* right;

    //defult constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    //initalize with course
    Node(Course aCourse):
        Node() {
            course = aCourse;
        
        }
};

class BinarySearchTree {
private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:
    BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseNumber);

};

BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}


void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root 
    this->inOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course)
{
    if (root == nullptr) {
        // root is equal to new node course
        root = new Node(course);
    }
    // else
    else {
        // add Node root and course
        this->addNode(root, course);
    }
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param Course course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // if node is larger then add to left
    if (node->course.courseNumber.compare(course.courseNumber) > 0) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        // else recurse down the left node
        else {
            this->addNode(node->left, course);
        }
    }
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        }
        else {
            // recurse down the left node
            this->addNode(node->right, course);
        }
    }
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseNumber) {

    Node* current = root;

    while (current != nullptr) {
        // if match found, return current course
        if (current->course.courseNumber.compare(courseNumber) == 0) {
            return current->course;
        }
        // if couseNumber is smaller than current node then traverse left
        if (courseNumber.compare(current->course.courseNumber) < 0) {
            current = current->left;
        }
        // else larger so traverse right
        else {
            current = current->right;
        }
    }
    Course course;
    return course;
}

void loadCourses(string cvsPath, BinarySearchTree* bst, istream& input, vector< vector<string> >& output) { 

    try {
        // loop to read rows of a CSV file
        string csvline;
            
            while (getline(input, csvline)) {

                stringstream csvStream(csvline);
                vector<string> csvColumn;
                string cvsItem;
                vector<string> csvPrereqs;
                Course course;
                string stringId;
                long tempId = NULL;
                //read line until comma ','
                while (getline(csvStream, cvsItem, ',')){

                    csvColumn.push_back(cvsItem);
                    //if there is only 2 perameters
                    if (csvColumn.size() == 2) {
                        
                        course.courseNumber = csvColumn[0];
                        course.courseName = csvColumn[1];
                        //for each charecter in courseName string-- convert to intager for courseId
                        for (char c : course.courseNumber) {
                            tempId = static_cast<int>(c);
                            //cout << tempId << endl;
                            stringId += to_string(tempId);
                            //cout << stringId << endl;
                        }

                        course.courseId = stringId;
                        stringId = "";
                    }
                    //if more then 3 prameters -- prerequisites
                    else if (csvColumn.size() >= 3) {
                        int numPrereqs = csvColumn.size() - 2;
                        course.courseNumber = csvColumn[0];
                        course.courseName = csvColumn[1];
                        
                        for (char c : course.courseNumber) {

                            tempId = static_cast<int>(c);
                            //cout << tempId << endl;
                            stringId += to_string(tempId);
                            //cout << stringId << endl;
                        }

                        course.courseId = stringId;
                        stringId = "";
                        
                        for (int i = 0; i < numPrereqs; i++) {

                            if (i == 0) {
                                course.coursePrereqs = csvColumn[i + 2];
                             }

                            else {
                                course.coursePrereqs += ", ";
                                course.coursePrereqs += csvColumn[i + 2];
                            }                            
                        }
                    }
                    else {
                        course.coursePrereqs = "No Prerequisits";
                    }  
                }
                //cout <<"ID: " << course.courseId << " Number: " << course.courseNumber << " Name: " << course.courseName << " Prereqs: " << course.coursePrereqs << endl;
                bst->Insert(course);   
            }
    }
    catch (Error& e) {
        std::cerr << e.what() << std::endl;
    }
   
}

//Displays specifide course
void displayCourse(Course course) {
    cout << course.courseNumber << ", " << course.courseName << endl;
    cout << "Prerequisites: " << course.coursePrereqs << endl;
    return;
}

//Displays all courses in order
void BinarySearchTree::inOrder(Node* node) {
    // If node does not equal nullptr
    if (node != nullptr) {

        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.courseName << endl;
        inOrder(node->right);

    }
}

int main(int argc, char* argv[]) {
    // File for courses
    string cvsPath = "ABCUniversityCourses.csv";

    ifstream myFile;
    typedef vector< vector<string> > csvVector;
    csvVector csvData;

    // Define a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;
    int choice = 0;
    //whle choice is not 9 -- userinput to EXIT
    while (choice != 9) {
        cout << "Welcome to the Course Planner." << endl;
        cout <<"" << endl;
        cout << "   1. Load Data Structure." << endl;
        cout << "   2. Print Course List." << endl;
        cout << "   3. Print Course." << endl;
        cout << "   9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do?" << endl;
        //cin.clear();
        cin >> choice;
        cin.clear();
        cin.ignore();
        cout << endl;
        

        switch (choice) {
        case 1: {
            string userFile = "";
            cout << "Enter file name. Case sensitive. Include file type (Ex: .csv or .txt)" << endl;
            cin >> userFile;
            cin.clear();
            cin.ignore();
            //if user input is empty, use default file
            if (userFile.compare("") == 0) {
                myFile.open(cvsPath);
                //cout << "File Open." << endl;

                if (!myFile.is_open()) {
                    perror("Error open");
                    exit(EXIT_FAILURE);
                }
                loadCourses(cvsPath, bst, myFile, csvData);
            }
            // Else load users file
            else {
                myFile.open(userFile);
                //cout << "File Open." << endl;

                if (!myFile.is_open()) {
                    perror("Error open");
                    exit(EXIT_FAILURE);
                }
                loadCourses(userFile, bst, myFile, csvData);
            }
            // close file
            myFile.close();
            break;
        }
         //Print all courses in order   
        case 2: {
            //cout << "-Print Course List-" << endl;
            cout << "Here is a sample schedule: " << endl;
            cout << endl;
            bst->InOrder();
            cout << endl;
            break;
        }

        //print specific course info
        case 3: {
            string userInput;
            string courseNumber;
            //cout << "-Print Course-" << endl;
            cout << "What course do you want to know about?" << endl;
            cin >> userInput;
            cin.clear();
            cin.ignore();
            cout << endl;
            //cin.ignore(numeric_limits<streamsize>::max(), '\n');
            for (int i = 0; i < userInput.size(); i++) {
                courseNumber += toupper(userInput[i]);
            }
            //courseNumber = toupper(userInput);
            course = bst->Search(courseNumber);

            //if course entered exists
            if (!course.courseId.empty()) {
                displayCourse(course);
                cout << endl;
            }
            //if course entered is not found
            else {
                cout << "Course Number: " << courseNumber << " not found." << endl;
                //cin.clear();
                cout << endl;
            }
            break;
        }
        
        //when 1, 2, 3, or 9 is not chosen
        default:
            cout << choice << "Invalid Input" << endl;
            break;
        }
        //cin >> choice;
         
    }
    cout << "Goodbye" << endl;
    return 0;
}
