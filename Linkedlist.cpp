// Linkedlist.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
using namespace std;

//#define BUILDING_DLL  // 在編譯 DLL 時定義該宏(把dll輸出，把這個解掉)

#ifdef BUILDING_DLL
#define DLL_API __declspec(dllexport)  // 導出符號
#else
#define DLL_API __declspec(dllimport)  // 導入符號
#endif

#ifndef BUILDING_DLL
int main()
{
    std::cout << "Hello World!\n";
}
#endif

template<typename T>
class Node
{
public:
    T val;
    Node<T>* next; //顯示模板(前面加，後面就要都加!)
    //Contructor
    Node(T val) :val(val), next(nullptr) {};
};

template <typename T>
class MyLinkedlist
{
private:
    Node<T>* Header;
public:
    /// <summary>
    /// Constructor
    /// </summary>
    MyLinkedlist() :Header(nullptr) {  
        std::cout << "Linkedlist is created" << endl;
    };
    /// <summary>
    /// DeepCopy
    /// </summary>
    /// <param name="_MyLinkedlist"></param>
    MyLinkedlist(const MyLinkedlist& _MyLinkedlist) {
        Header = nullptr;  // 開始時設置為空

        Node<T>* temp = _MyLinkedlist.Header; // 遍歷原鏈表
        // 添加到新鏈表的尾部
        while (temp != nullptr) {
            Append(temp->val); //裡面會把Header指向new出來的新節點
            temp = temp->next;
        }
        cout << "Linked List Deep Copied" << endl;
    };

    /// <summary>
    /// 在list末端增加元素 
    /// </summary>
    /// <param name="_val"></param>
    void Append(T _val) {
        Node<T>* newNode = new Node<T>(_val); //創建記憶體
        if (Header == nullptr) {
            Header = newNode;  // 如果鏈表是空的，將新節點設為頭節點
        }
        else {
            Node<T>* temp = Header;
            // 找到鏈表的最後一個節點
            while (temp->next != nullptr) {
                temp = temp->next;  
            }
            temp->next = newNode;  // 將新節點連接到最後
        }
    }

    /// <summary>
    /// 刪除最後一個元素
    /// </summary>
    void pop() {
        //無節點
        if (!Header) return;

        //如果只有一個節點
        if (!Header->next)
        {
            delete Header;
            Header = nullptr;
            return;
        }

        //刪除最後一個節點
        Node<T>* temp = Header;
        while (temp->next && temp -> next -> next){
            temp = temp->next;
        }

        delete temp->next;
        temp->next = nullptr;
    }

    /// <summary>
    /// 刪除節點
    /// </summary>
    /// <param name="_val"></param>
    /// <param name="IsAll(delete all)"></param>
    void DelectNode(T _val,bool IsAll = true) {
        if (!Header) return;

        //Header的值 = _val
        Node<T>* temp = Header;
        if (Header->val == _val) {
            Header = Header -> next;
            delete temp;
            if (!IsAll) return ;
        }

        //找其他值的
        while (temp -> next) {
            if (temp-> next -> val == _val) {
                Node* delnode = temp->next;
                temp->next = temp->next->next;
                delete delnode;
                if (!IsAll) return;
            }
            temp = temp->next;
        }
    }

    /// <summary>
    /// 插入值
    /// </summary>
    /// <param name="_location"></param>
    /// <param name="_val"></param>
    void Insert(int _location, T _val) {
        // 檢查 _location 是否有效
        if (_location < 0) {
            std::cout << "Invalid location!" << std::endl;
            throw std::out_of_range("Location cannot be negative.");
        }

        Node<T>* NewNode = new Node<T>(_val);

        // 插入到頭部的位置 (_location == 0)(容易忘記想到)
        if (_location == 0) {
            NewNode->next = Header;
            Header = NewNode;
            throw std::out_of_range("Location out of bounds.");
        }

        // 插入到非頭部位置 (_location > 0)
        Node<T>* temp = Header;
        int iStep = 0;

        // 遍歷到插入位置的前一個節點
        while (temp && ++iStep != _location) {
            temp = temp->next;
        }

        // 如果遍歷完了還找不到位置，表示位置超出了鏈表的長度，應拋出異常
        if (!temp) {
            std::cout << "Invalid location!" << std::endl;
            throw std::out_of_range("Location out of bounds.");
        }

        // 插入新節點
        NewNode->next = temp->next;
        temp->next = NewNode;
    }

    void RemoveAt(int _location) {
        if (_location < 0) throw std::out_of_range("Invalid location!");

        Node<T>* temp = Header;
        if (_location == 0) {
            if(Header) Header = Header->next;
            delete temp;
            return;
        }

        int istep = 0;
        while (temp && ++istep != _location) {
            temp = temp->next;
        }

        if (!temp || !temp -> next) throw std::out_of_range("Invalid location!");
        Node<T>* deltemp = temp -> next;
        temp -> next = temp->next->next;
        delete deltemp;
    }

    string printfAll() {
        if (!Header) return "";

        MyType WhichType = MyType::arithmetic;
        //value = true代表示這類型；value = false代表不是
        if constexpr (std::is_arithmetic<T>::value) {
            // 如果 T 是數字類型，直接轉換
            WhichType = MyType::arithmetic;
        }
        else if constexpr (std::is_same<T, std::string>::value) {
            // 如果 T 是 std::string，直接返回
            WhichType = MyType::MyString;
        }
        else {
            // 其他情況，返回一個預設的字符串表示
            WhichType = MyType::Other;
            return "Unsupported type";
        }

        Node<T>* temp = Header; string _sAns = "";
        while (temp) {
            switch (WhichType)
            {
            case MyType::arithmetic:
                _sAns += to_string(temp->val);//前面有namespace後面就不用加std::
                break;
                case MyType::MyString:
                _sAns += temp->val;
                break;
            default:
                break;
            }
            temp = temp->next;
        }      
        return _sAns;
    }

    enum MyType{
        arithmetic,
        MyString,
        Other,
    };

    ~MyLinkedlist() {
        Node<T>* temp = Header;
        while (temp) {
            Node<T>* nextNode = temp->next;  // 保存下一個節點的指標
            delete temp;  // 釋放當前節點
            temp = nextNode;  // 移動到下一個節點
        }
        Header = nullptr;  // 最後將Header設為nullptr，確保指針被清空
    }


    // 顛倒排序 reverse()
};
    

// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
