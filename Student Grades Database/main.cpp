#include <iostream>
#include "Student.hpp"
#include "StudentRecord.hpp"
#include "BTreeNode.hpp"
#include "Stack.hpp"
#include "Queue.hpp"

using namespace std;

int SearchStudent (int ID, BTreeNode<Student>* Tree, bool& valid);
void PrintStudent (int ID, StudentRecord* Database, BTreeNode<Student>* Tree);
void UpdateStudentGrades (int ID, int* grades, StudentRecord* Database, BTreeNode<Student>* Tree);
void InsertStudent (int ID, char* Name, int* grades, StudentRecord* Database, BTreeNode<Student>*& Tree);
void DeleteStudent (int ID, StudentRecord* Database, BTreeNode<Student>* Tree);
void List (BTreeNode<Student>* Tree);
void PrintTree (BTreeNode<Student>* Tree);

// if one of the student data in the node is empty, the corresponding ID is the default ID
const int defaultID = -1;
const int database_size = 100;

int main()
{
    StudentRecord Student_Record_Database[database_size];

    BTreeNode<Student>* Student_Info_Tree = NULL;

    int grade1[] = {1, 2, 3};
    char name1[] = "Student1";
    InsertStudent(5, name1, grade1, Student_Record_Database, Student_Info_Tree);

    int grade2[] = {4, 5, 6};
    char name2[] = "Student2";
    InsertStudent(3, name2, grade2, Student_Record_Database, Student_Info_Tree);

    int grade3[] = {7, 8, 9};
    char name3[] = "Student3";
    InsertStudent(4, name3, grade3, Student_Record_Database, Student_Info_Tree);

    int grade4[] = {10, 11, 12};
    char name4[] = "Student4";
    InsertStudent(7, name4, grade4, Student_Record_Database, Student_Info_Tree);

    int grade5[] = {13, 14, 15};
    char name5[] = "Student5";

    InsertStudent(6, name5, grade5, Student_Record_Database, Student_Info_Tree);
    InsertStudent(12, name5, grade5, Student_Record_Database, Student_Info_Tree);
    InsertStudent(25, name5, grade5, Student_Record_Database, Student_Info_Tree);
    InsertStudent(26, name5, grade5, Student_Record_Database, Student_Info_Tree);
    InsertStudent(11, name5, grade5, Student_Record_Database, Student_Info_Tree);
    InsertStudent(16, name5, grade5, Student_Record_Database, Student_Info_Tree);
    InsertStudent(14, name5, grade5, Student_Record_Database, Student_Info_Tree);
    InsertStudent(17, name5, grade5, Student_Record_Database, Student_Info_Tree);

    PrintStudent(12, Student_Record_Database, Student_Info_Tree);
    PrintStudent(7, Student_Record_Database, Student_Info_Tree);
    PrintStudent(26, Student_Record_Database, Student_Info_Tree);
    PrintStudent(3, Student_Record_Database, Student_Info_Tree);

    PrintStudent(26, Student_Record_Database, Student_Info_Tree);
    UpdateStudentGrades(26, grade2, Student_Record_Database, Student_Info_Tree);
    PrintStudent(26, Student_Record_Database, Student_Info_Tree);
    DeleteStudent(26, Student_Record_Database, Student_Info_Tree);
    PrintStudent(26, Student_Record_Database, Student_Info_Tree);

    PrintTree(Student_Info_Tree);
    List(Student_Info_Tree);

    return 0;
}

int SearchStudent (int ID, BTreeNode<Student>* Tree, bool& valid)
{
    // if the Tree points to NULL, return -1
    if (Tree == NULL)
    {
        valid = false;
        return -1;
    }

    // if first key is the searched ID, return it
    if(Tree->data[0].studentID == ID)
    {
        valid = Tree->data[0].record_valid;
        return Tree->data[0].index;
    }
    // if first key is greater than the searched ID, recurse on the first child of the node
    else if (Tree->data[0].studentID > ID)
    {
        return SearchStudent(ID, Tree->Children[0], valid);
    }
    // if first key is less than the searched ID, look for the second key
    // if second data is empty, recurse on the second child of the node
    else if (Tree->data[1].studentID == defaultID)
    {
        return SearchStudent(ID, Tree->Children[1], valid);
    }
    // if second key is the searched ID, return it
    else if (Tree->data[1].studentID == ID)
    {
        valid = Tree->data[1].record_valid;
        return Tree->data[1].index;
    }
    // if second key is greater than the searched ID, recurse on the second child of the node
    else if (Tree->data[1].studentID > ID)
    {
        return SearchStudent(ID, Tree->Children[1], valid);
    }
    // if second key is less than the searched ID, recurse on the third child of the node
    else
    {
        return SearchStudent(ID, Tree->Children[2], valid);
    }
}

void PrintStudent (int ID, StudentRecord* Database, BTreeNode<Student>* Tree)
{
    bool success;
    int index;

    index = SearchStudent(ID, Tree, success);

    // if not found, print error message
    if (success == false)
    {
        cerr << "No record is found with the given information." << endl;
    }
    else // if found, print student name and the grades
    {
        cout << "Student Name: " << Database[index].studentName;
        for (int i = 0; i < 3; i++)
        {
            cout << ", Grade " << (i + 1) << ": " << Database[index].studentGrades[i];
        }
        cout << endl;
    }

    return;
}

void UpdateStudentGrades (int ID, int* grades, StudentRecord* Database, BTreeNode<Student>* Tree)
{
    bool success;
    int index;

    // first, search the student to know if it exists in the database
    index = SearchStudent(ID, Tree, success);

    // if not found, print error message
    if (success == false)
    {
        cerr << "No record is found with the given information." << endl;
    }
    else // if found, update the grades of the student
    {
        for (int i = 0; i < 3; i++)
        {
            Database[index].studentGrades[i] = grades[i];
        }
    }

    return;
}

void InsertStudent (int ID, char* Name, int* grades, StudentRecord* Database, BTreeNode<Student>*& Tree)
{
    bool success;

    // first, search the student to know if it exists in the database
    SearchStudent(ID, Tree, success);

    // if already exists, print error message
    if (success == true)
    {
        cerr << "Record already exists in the database." << endl;
        return;
    }

    // look for first empty location in the database array
    int index = 0;
    while (index < database_size && Database[index].valid == true)
        index++;

    // if empty location is not found
    if (index == database_size)
    {
        cerr << "Database is full, no empty location." << endl;
        return;
    }

    // if empty location is found, insert the Student Record to the database
    StudentRecord temp(true, ID, Name, grades);

    Database[index] = temp;

    // if the root is NULL, create a new node, and insert student to the first data
    if (Tree == NULL)
    {
        Tree = new BTreeNode<Student>;
        Student temp(ID, true, index);
        Tree->data[0] = temp;
        return;
    }

    // if it is not NULL, find the corresponding leaf to insert the data

    // stack implemented in HW 1 is used to store the nodes passed until the corresponding leaf is found
    // the node stack will contain the corresponding leaf, the full nodes just passed before the corresponding leaf, and the one above them
    Stack<BTreeNode<Student>*> TreeNodeStack;

    // the children no stack will contain the children no's passed while finding the corresponding leaf
    Stack<int> ChildrenNoStack;

    // flag shows that all the nodes passed to reach the leaf are full
    // which means that the root will also split
    int flag = 1;

    BTreeNode<Student>* tempNode = Tree;

    // loop until a leaf is found, and push the necessary nodes and children numbers to the stacks
    // if an equal ID is found, the record_valid value is made "true", and the function returns
    while (tempNode != NULL)
    {
        // if the node is not full (second data doesn't exist), clear the stacks
        if (tempNode->data[1].studentID == defaultID)
        {
            TreeNodeStack.ClearStack();
            ChildrenNoStack.ClearStack();
            flag = 0;
        }

        // push the node to the stack
        TreeNodeStack.Push(tempNode);

        // if first key is the ID to be inserted, its record_valid must be false, overwrite it
        if(tempNode->data[0].studentID == ID)
        {
            Student temp(ID, true, index);
            tempNode->data[0] = temp;
            return;
        }
        // if first key is greater than the ID to be inserted, continue from the first child
        else if (tempNode->data[0].studentID > ID)
        {
            tempNode = tempNode->Children[0];
            ChildrenNoStack.Push(0);
        }
        // if first key is less than the ID to be inserted, look for the second key
        // if the second data doesn't exist, continue from the second child
        else if (tempNode->data[1].studentID == defaultID)
        {
            tempNode = tempNode->Children[1];
            ChildrenNoStack.Push(1);
        }
        // if second key is the ID to be inserted, its record_valid must be false, overwrite it
        else if (tempNode->data[1].studentID == ID)
        {
            Student temp(ID, true, index);
            tempNode->data[1] = temp;
            return;
        }
        // if second key is greater than the ID to be inserted, continue from the second child of the node
        else if (tempNode->data[1].studentID > ID)
        {
            tempNode = tempNode->Children[1];
            ChildrenNoStack.Push(1);
        }
        // if second key is less than the ID to be inserted, continue from the third child of the node
        else
        {
            tempNode = tempNode->Children[2];
            ChildrenNoStack.Push(2);
        }
    }

    // while iterating upwards three things go up, two nodes and one data
    BTreeNode<Student>* carriedNode1 = NULL;
    BTreeNode<Student>* carriedNode2 = NULL;
    Student carriedData(ID, true, index);

    // iterate until the stacks are empty
    while (TreeNodeStack.StackEmpty() == 0)
    {
        BTreeNode<Student>* TreeNode = TreeNodeStack.Pop();
        int ChildrenNo = ChildrenNoStack.Pop();

        // if the uppermost node is a half-full node
        if ((TreeNodeStack.StackEmpty() == 1) && (flag == 0))
        {
            if (ChildrenNo == 0)
            {
                TreeNode->data[1] = TreeNode->data[0];
                TreeNode->data[0] = carriedData;

                TreeNode->Children[2] = TreeNode->Children[1];
                TreeNode->Children[1] = carriedNode2;
                TreeNode->Children[0] = carriedNode1;
            }
            else // if (ChildrenNo == 1)
            {
                TreeNode->data[1] = carriedData;
                TreeNode->Children[1] = carriedNode1;
                TreeNode->Children[2] = carriedNode2;
            }
        }
        else
        {
            if (ChildrenNo == 0)
            {
                BTreeNode<Student>* tempNode1 = new BTreeNode<Student>;
                tempNode1->data[0] = carriedData;
                tempNode1->Children[0] = carriedNode1;
                tempNode1->Children[1] = carriedNode2;

                BTreeNode<Student>* tempNode2 = new BTreeNode<Student>;
                tempNode2->data[0] = TreeNode->data[1];
                tempNode2->Children[0] = TreeNode->Children[1];
                tempNode2->Children[1] = TreeNode->Children[2];

                carriedNode1 = tempNode1;
                carriedNode2 = tempNode2;
                carriedData = TreeNode->data[0];

                delete TreeNode;
            }
            else if (ChildrenNo == 1)
            {
                BTreeNode<Student>* tempNode1 = new BTreeNode<Student>;
                tempNode1->data[0] = TreeNode->data[0];
                tempNode1->Children[0] = TreeNode->Children[0];
                tempNode1->Children[1] = carriedNode1;

                BTreeNode<Student>* tempNode2 = new BTreeNode<Student>;
                tempNode2->data[0] = TreeNode->data[1];
                tempNode2->Children[0] = carriedNode2;
                tempNode2->Children[1] = TreeNode->Children[2];

                carriedNode1 = tempNode1;
                carriedNode2 = tempNode2;
                // carriedData doesn't change

                delete TreeNode;
            }
            else // if (ChildrenNo == 2)
            {
                BTreeNode<Student>* tempNode1 = new BTreeNode<Student>;
                tempNode1->data[0] = TreeNode->data[0];
                tempNode1->Children[0] = TreeNode->Children[0];
                tempNode1->Children[1] = TreeNode->Children[1];

                BTreeNode<Student>* tempNode2 = new BTreeNode<Student>;
                tempNode2->data[0] = carriedData;
                tempNode2->Children[0] = carriedNode1;
                tempNode2->Children[1] = carriedNode2;

                carriedNode1 = tempNode1;
                carriedNode2 = tempNode2;
                carriedData = TreeNode->data[1];

                delete TreeNode;
            }
        }
    }

    // if the root is divided, the carried nodes and the carried data make up the root
    if (flag == 1)
    {
        Tree = new BTreeNode<Student>;
        Tree->data[0] = carriedData;
        Tree->Children[0] = carriedNode1;
        Tree->Children[1] = carriedNode2;
    }

    return;
}

void DeleteStudent (int ID, StudentRecord* Database, BTreeNode<Student>* Tree)
{
    // if the Tree points to NULL, return
    if (Tree == NULL)
    {
        return;
    }

    // if first key is the ID looked for, corresponding valid and record_valid is set to false
    if (Tree->data[0].studentID == ID)
    {
        if (Tree->data[0].record_valid == true)
        {
            Tree->data[0].record_valid = false;
            Database[Tree->data[0].index].valid = false;
        }
    }
    // if first key is greater than the searched ID, recurse on the first child of the node
    else if (Tree->data[0].studentID > ID)
    {
        DeleteStudent(ID, Database, Tree->Children[0]);
    }
    // if first key is less than the searched ID, look for the second key
    // if the second data doesn't exist, recurse on the second child of the node
    else if (Tree->data[1].studentID == defaultID)
    {
        DeleteStudent(ID, Database, Tree->Children[1]);
    }
    // if second key is the ID looked for, corresponding valid and record_valid is set to false
    else if (Tree->data[1].studentID == ID)
    {
        if (Tree->data[1].record_valid == true)
        {
            Tree->data[1].record_valid = false;
            Database[Tree->data[1].index].valid = false;
        }
    }
    // if second key is greater than the searched ID, recurse on the second child of the node
    else if (Tree->data[1].studentID > ID)
    {
        DeleteStudent(ID, Database, Tree->Children[1]);
    }
    // if second key is less than the searched ID, recurse on the third child of the node
    else
    {
        DeleteStudent(ID, Database, Tree->Children[2]);
    }

    return;
}

void List (BTreeNode<Student>* Tree)
{
    // if it points to NULL, return without printing anything
    if (Tree == NULL)
    {
        return;
    }

    // recurse on first child of the node
    List (Tree->Children[0]);

    // if student in first key exists, print its ID
    if (Tree->data[0].record_valid)
    {
        cout << Tree->data[0].studentID << endl;
    }

    // recurse on second child of the node
    List (Tree->Children[1]);

    // if student in second key exists, print its ID
    if (Tree->data[1].record_valid)
    {
        cout << Tree->data[1].studentID << endl;
    }

    // recurse on third child of the node
    List (Tree->Children[2]);

    return;
}

void PrintTree (BTreeNode<Student>* Tree)
{
    // if it points to NULL, return without printing anything
    if (Tree == NULL)
    {
        return;
    }

    // queue implemented in HW 1 is used to order the nodes
    Queue<BTreeNode<Student>*> TreeNodeQueue;

    TreeNodeQueue.Insert(Tree);

    BTreeNode<Student>* temp;

    // loop until all the nodes are traversed
    while(TreeNodeQueue.QEmpty() == 0)
    {
        temp = TreeNodeQueue.Delete();

        // if the node doesn't point to NULL, print its data and insert its children to the queue
        if (temp != NULL)
        {
            for (int i = 0; i < 2; i++)
            {
                // if student in i'th key exists, print its ID and record_valid
                if (temp->data[i].studentID != defaultID)
                {
                    cout << "Student ID: " << temp->data[i].studentID << ", record_valid: " << temp->data[i].record_valid;
                    // if record_valid is true, print the index
                    if (temp->data[i].record_valid)
                    {
                        cout << ", index: " <<temp->data[i].index;
                    }
                    cout << endl;
                }
            }

            TreeNodeQueue.Insert(temp->Children[0]);
            TreeNodeQueue.Insert(temp->Children[1]);
            TreeNodeQueue.Insert(temp->Children[2]);
        }
    }

    return;
}
