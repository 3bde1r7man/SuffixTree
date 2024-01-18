#include <cstdlib> 
#include <cstdio> 
#include <cstring>
#include <iostream>

template<class T>
class List {
private:
    T* arr{};
    int size{};
    int capacity{};

    void reallocate()
    {
        capacity *= 2;

        T* arr2 = new T[capacity]{};

        for (int i = 0; i < size; ++i)
            arr2[i] = arr[i];

        T* temp = arr2;
        arr2 = arr;
        arr = temp;

        delete[] arr2;
    }

public:
    List()
    {
        this->capacity = 27;
        arr = new T[capacity]{};
    }

    int get_size()
    {
        return size;
    }

    T get_element(int idx)
    {
        return arr[idx];
    }

    T find(char ch, char* text)
    {
        for (int i = 0; i < size; ++i)
        {
            if (text[arr[i]->start] == ch)
            {
                return arr[i];
            }
        }
        return nullptr;
    }

    T back()
    {
        return arr[size - 1];
    }

    void insert(T val)
    {
        if (size == capacity)
            reallocate();
        arr[size] = val;
        size++;
    }

    T pop_back()
    {
        T back = arr[size - 1];
        size--;
        return back;
    }

    int get_min_start(int text_len)
    {
        int mn_start = text_len;
        for (int i = 0; i < size; i++)
        {
            if (arr[i]->start < mn_start)
            {
                mn_start = arr[i]->start;
            }
        }
        return mn_start;
    }

    bool empty()
    {
        return size == 0;
    }

    ~List()
    {
        delete[] arr;
        arr = nullptr;
    }
};

// Node class for the suffix tree each node have a list of children and the start index of the suffix and the id of the suffix
class Node
{
public:
    List<Node*> children;
    int start;
    int id;
    Node() : start(-1), id(-1) {};
    Node(int start, int id) : start(start), id(id) {};
};


class SuffixTree
{
public:
    Node* root;
    char* text;
    SuffixTree(char* text)
    {
        this->text = text;
        root = new Node();
        Node* curr = root;

        //build suffix tree
        for (int i = 0; i < strlen(text); i++)
        {
            curr = root;
            Node* prev = curr;

            // find the letter in the curr node children
            Node* temp = curr->children.find(text[i], text);

            // if the letter is not in the curr node add it 
            if (temp == nullptr)
            {
                curr->children.insert(new Node(i, i));
            }
            // go deeper in the tree check the rest of the suffix
            else
            {
                int start = temp->start;
                int left = i;
                int oldleft = left;
                Node* node1 = new Node();
                Node* node2 = new Node();

                // iterate on the rest of the nodes in this suffix path
                while (true)
                {
                    // calc the mn start for all children of the temp
                    int mn_start = temp->children.get_min_start(strlen(text));

                    bool flag = true;
                    bool swith = true;

                    int curr_start = start;

                    // mach the rest of your new_suffix with the old suffix in this path
                    // if there is a miss match we will split around this node
                    // and add our suffix  
                    for (int i = 0; i < mn_start - curr_start; i++)
                    {
                        // check if the two chars matche
                        if (text[start] == text[left])
                        {
                            flag = false;
                            left++;
                            start++;
                        }

                        // miss match happened
                        else
                        {
                            // flag = true ==> there is no need to split just insert
                            if (flag)
                            {
                                node2->start = left;
                                node2->id = oldleft;
                                prev->children.insert(node2);
                                flag = false;
                                swith = false;
                            }

                            // split the node
                            else
                            {
                                node1->start = start;
                                node1->id = temp->id;
                                temp->id = -1;

                                // remove children of temp and put them into node1
                                while (!temp->children.empty())
                                {
                                    node1->children.insert(temp->children.pop_back());
                                }

                                node2->start = left;
                                node2->id = oldleft;

                                // insert the splited nodes into the temp children
                                temp->children.insert(node1);
                                temp->children.insert(node2);

                                // mark flag as false to stop traversing
                                flag = false;
                                swith = false;
                            }
                            break;
                        }
                    }

                    // stop traversing if you inserted the suffix                    
                    if (!flag && !swith)
                    {
                        break;
                    }

                    // traverse on the next node in the tree
                    Node* child = temp->children.find(text[left], text);
                    prev = temp;
                    if (child != nullptr)
                    {
                        temp = child;
                        start = temp->start;
                    }
                    else
                    {
                        temp = temp->children.find(text[start], text);
                    }
                }
            }
        }
    }

    void Search(char* pattern, int left = 0, Node* curr = nullptr, bool found = 0)
    {
        // if curr iss null then i should begin from the root
        if (curr == nullptr)
        {
            curr = root;
        }

        // if the pattern still not found
        if (!found)
        {
            Node* left_child = curr->children.find(pattern[left], text);

            if (left_child == NULL)
            {
                std::cout << "not found";
                return;
            }

            int patternI = left;
            int mn_start = left_child->children.get_min_start(strlen(text));

            int right = left_child->start;
            int curr_start = right;

            // match the pattern with current suffix
            for (int i = 0; i < mn_start - curr_start; i++)
            {
                if (patternI >= strlen(pattern))
                {
                    break;
                }

                // if still pattern match the suffix move on the next char
                if (pattern[patternI] == text[right])
                {
                    patternI++;
                    right++;
                }

                // otherwise pattern not found in this tree
                else
                {
                    std::cout << "not found";
                    return;
                }
            }

            // if we finsished the pattern then we found it
            if (patternI == strlen(pattern))
            {
                Search(pattern, patternI, left_child, true);
                return;
            }

            // we need to go deeper in the tree
            else
            {
                Search(pattern, patternI, left_child, false);
            }
        }
        // we found the pattern
        else if (found)
        {
            // Im a leaf so I have the id and no one under me 
            if (curr->id != -1)
            {
                std::cout << curr->id;
                return;
            }

            for (int j = 0; j < curr->children.get_size(); j++)
            {
                // my child have the suffix id my child is a leaf
                Node* child = curr->children.get_element(j);
                if (child->id != -1)
                {
                    std::cout << child->id << ' ';
                }

                // go deeper in the tree
                else
                {
                    Search(pattern, 0, child, true);
                }
            }
        }
    }
};



int main()
{
    // Construct a suffix tree containing all suffixes of "bananabanaba$"
    char c[] = "bananabanaba$";
    SuffixTree t(c);
    std::cout << "=== 1. tree = bananabanaba$ ===\n";
    char s[] = "a";
    std::cout << "1. " << s << " ==> ";
    t.Search(s); // prints : 3 7 1 5 9 11
    char s1[] = "ban";
    std::cout << "\n2. " << s1 << " ==> ";
    t.Search(s1); // prints : 6 0
    char s2[] = "naba";
    std::cout << "\n3. " << s2 << " ==> ";
    t.Search(s2); // prints : 4 8
    char s3[] = "naz";
    std::cout << "\n4. " << s3 << " ==> ";
    t.Search(s3); // prints : not found

    // Construct a suffix tree containing all suffixes of "banana$"
    std::cout << "\n\n=== 2. tree = banana$ ===\n";
    char c2[] = "banana$";
    SuffixTree t2(c2);
    char s4[] = "ban";
    std::cout << "1. " << s4 << " ==> ";
    t2.Search(s4); // prints : 0
    char s5[] = "anana";
    std::cout << "\n2. " << s5 << " ==> ";
    t2.Search(s5); // prints : 1
    char s6[] = "na";
    std::cout << "\n3. " << s6 << " ==> ";
    t2.Search(s6); // prints : 2 4

    // Construct a suffix tree containing all suffixes of "ababba$"
    std::cout << "\n\n=== 3. tree = ababba$ ===\n";
    char c3[] = "ababba$";
    SuffixTree t3(c3);
    char s7[] = "ab";
    std::cout << "1. " << s7 << " ==> ";
    t3.Search(s7); // prints : 2 0
    char s8[] = "ba";
    std::cout << "\n2. " << s8 << " ==> ";
    t3.Search(s8); // prints : 1 4
    char s9[] = "abba";
    std::cout << "\n3. " << s9 << " ==> ";
    t3.Search(s9); // prints : 2

    // Construct a suffix tree containing all suffixes of "btababtbtabt$"
    std::cout << "\n\n=== 4. tree = btababtbtabt$ ===\n";
    char c4[] = "btababtbtabt$";
    SuffixTree t4(c4);
    char s10[] = "ababtbtabt";
    std::cout << "1. " << s10 << " ==> ";
    t4.Search(s10);  // prints : 2
    char s11[] = "btbtabt";
    std::cout << "\n2. " << s11 << " ==> ";
    t4.Search(s11); // prints : 5
    char s12[] = "tt";
    std::cout << "\n3. " << s12 << " ==> ";
    t4.Search(s12); // prints : not found

    // Construct a suffix tree containing all suffixes of "tomato$"
    std::cout << "\n\n=== 5. tree = tomato$ ===\n";
    char c5[] = "tomato$";
    SuffixTree t5(c5);
    char s13[] = "tom";
	std::cout << "1. " << s13 << " ==> ";
    t5.Search(s13); // prints : 0
    char s14[] = "a";
    std::cout << "\n2. " << s14 << " ==> ";
    t5.Search(s14); // prints : 3
    char s15[] = "to";
    std::cout << "\n3. " << s15 << " ==> ";
    t5.Search(s15); // prints : 0 4

    // Construct a suffix tree containing all suffixes of "potato$"
    std::cout << "\n\n=== 6. tree = potato$ ===\n";
    char c6[] = "potato$";
    SuffixTree t6(c6);
    char s16[] = "to";
    std::cout << "1. " << s16 << " ==> ";
    t6.Search(s16); // prints : 4
    char s17[] = "tato";
    std::cout << "\n2. " << s17 << " ==> ";
    t6.Search(s17); // prints : 2
    char s18[] = "at";
    std::cout << "\n3. " << s18 << " ==> ";
    t6.Search(s18); // prints : 3

    // Construct a suffix tree containing all suffixes of "batatatattta$"
    std::cout << "\n\n=== 7. tree = batatatattta$ ===\n";
    char c7[] = "batatatattta$";
    SuffixTree t7(c7);
    char s19[] = "atata";
    std::cout << "1. " << s19 << " ==> ";
    t7.Search(s19); // prints : 1 3
    char s20[] = "ttt";
    std::cout << "\n2. " << s20 << " ==> ";
    t7.Search(s20); // prints : 8
    char s21[] = "tat";
    std::cout << "\n3. " << s21 << " ==> ";
    t7.Search(s21); // prints : 2 4 6

    // Construct a suffix tree containing all suffixes of "rooomeeer$"
    std::cout << "\n\n=== 8. tree = rooomeeer$ ===\n";
    char c8[] = "rooomeeer$";
    SuffixTree t8(c8);
    char s22[] = "ooo";
    std::cout << "1. " << s22 << " ==> ";
    t8.Search(s22); // prints : 1
    char s23[] = "mee";
    std::cout << "\n2. " << s23 << " ==> ";
    t8.Search(s23); // prints : 4
    char s24[] = "eeer";
    std::cout << "\n3. " << s24 << " ==> ";
    t8.Search(s24); // prints : 5

    // Construct a suffix tree containing all suffixes of "babaritooopapit$"
    std::cout << "\n\n=== 9. tree = babaritooopapit$ ===\n";
    char c9[] = "babaritooopapit$";
    SuffixTree t9(c9);
    char s25[] = "it";
    std::cout << "1. " << s25 << " ==> ";
    t9.Search(s25); // prints : 5 13
    char s26[] = "pap";
    std::cout << "\n2. " << s26 << " ==> ";
    t9.Search(s26); // prints : 10
    char s27[] = "babar";
    std::cout << "\n3. " << s27 << " ==> ";
    t9.Search(s27); // prints : 0

    // Construct a suffix tree containing all suffixes of "bewbewmeawmeaw$"
    std::cout << "\n\n=== 10. tree = bewbewmeawmeaw$ ===\n";
    char c10[] = "bewbewmeawmeaw$";
    SuffixTree t10(c10);
    char s28[] = "bew";
    std::cout << "1. " << s28 << " ==> ";
    t10.Search(s28); // prints : 0 3
    char s29[] = "meaw"; 
    std::cout << "\n2. " << s29 << " ==> ";
    t10.Search(s29); // prints : 10 6
    char s30[] = "bewm";
    std::cout << "\n3. " << s30 << " ==> ";
    t10.Search(s30); // prints : 3

    return 0;
}