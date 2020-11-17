//p5.cpp
//In this program we are finding the count of occurrence of words in ulyss12.txt
//and storing in a hash table. User can input words to find occurrence of it


#include<iostream>
#include<fstream>
#include<cctype>

using namespace std;

struct NODE
{
    string word;
    int count;
    NODE* next;
};
string preProcess(string word,unsigned MIN_WORD);
//Processing the word before hashing
int readFile(string fileName,const int MAX,const unsigned MIN_WORD,NODE* table[]);
//reading text file
unsigned hashFunction(string word,int tableSize);
//performing hashing
int store(unsigned int key,NODE* table[],string word);
//storing data in the hash table
int checkCount(string word,NODE* table[],const unsigned int MIN,const int MAX);
//check occurrence of a work in the text file
void hashingResults(int collisions, NODE* table[],const int MAX);
//results of hashing like load factor, empty slots etc.
void dealocatingMemory(const int MAX, NODE* table[]);
void displayFunction(NODE* table[]);
//displaying output

int main()
{
    const int MAX = 33827;//size of hash table
    const unsigned MIN_WORD = 5; //min word length
    const string FILE_NAME = "ulyss12.txt";
    NODE* list[MAX]; //pointer array
    string inputWord;
    int count;
    int collisions;
    for(int i=0; i<MAX; i++)
    {
        list[i] = nullptr;//initializing pointer array
    }
    collisions = readFile(FILE_NAME,MAX,MIN_WORD,list);//READ FILE
    hashingResults(collisions,list,MAX);//RESULTS OF HASH
    displayFunction(list);//display some words
    cout<<"\n\nEnter a word to find its occurrence or press 0 to exit ";
    cin>>inputWord;
    while(inputWord != "0")
    {
        count = checkCount(inputWord,list,MIN_WORD,MAX);//perform check count
        cout<<"\n"<<inputWord<<" occurred : "<<count<<" times";
        cout<<"\n\nTo continue type a word or press 0 to exit  ";
        cin>>inputWord;
    }
    dealocatingMemory(MAX,list);
    return 0;
}
void displayFunction(NODE* table[]){
    NODE* ptr;
    cout<<"press enter to see a list of some of the words...";
    cin.get();
    for(int i=11; i<50; i++)
    {
        ptr = table[i];
        while(ptr!=nullptr)
        {
            cout<<ptr->word<<' '<< ptr->count<<"\n";
            ptr=ptr->next;
        }
    }
}
int checkCount(string word,NODE* table[],const unsigned int MIN_WORD,const int MAX)
{
    string resultWord;
    unsigned int index;
    resultWord = preProcess(word,MIN_WORD);//PRE-PROCESS THE WORD
    if(resultWord.length() > MIN_WORD) //check processed word length
    {
        index = hashFunction(resultWord,MAX);//find index after hashing the word
        if(table[index] != nullptr) //check index in hash table is not null
        {
            if(table[index]->word == word)
            {
                return table[index]->count;
            }
            else if(table[index]->next != nullptr)//if linkedlist at the
                                        //index is present, then traverse it
            {
                NODE* ptr = table[index];

                while(ptr!= nullptr)
                {
                    if(ptr->word  == word)
                    {
                        return ptr->count;
                    }
                    ptr=ptr->next;
                }
            }
        }
    }
    return 0;
}
//de-allocating memory
void dealocatingMemory(const int MAX, NODE* table[])
{
    NODE* ptr;
    NODE* delptr;
    for(int i=0; i<MAX; i++)
    {
        if(table[i] != nullptr)
        {
            ptr = table[i];
            while(ptr != nullptr)
            {
                delptr = ptr;
                ptr = ptr->next;
                delete delptr;
            }
        }
    }
}

//performing calculation to find load factor etc.
void hashingResults(int collisions,NODE* table[],const int MAX)
{
    int uniqueWordCount=0;
    int totalWordCount = 0;
    NODE* ptr,*temp;
    NODE* delptr;
    int nullPtrCount=0;
    int nodeCount=0;
    int maxNodeCount=0;
    for(int i=0; i<MAX; i++)
    {
        if(table[i] == nullptr)
        {
            nullPtrCount++;
        }
        else
        {
            nodeCount=0;
            ptr=table[i];
            while(ptr!=nullptr)
            {
                nodeCount++;
                uniqueWordCount++;
                totalWordCount += ptr->count;
                temp = ptr->next;
                ptr=ptr->next;
            }
            if(nodeCount > maxNodeCount)
            {
                maxNodeCount = nodeCount;
            }
        }
    }
    cout<< "Total collisions = "<<collisions<<"\n";
    cout<<"Total words stored: "<<totalWordCount<<"\n";
    cout<<"Unique words stored: "<<uniqueWordCount<<"\n";
    cout<<"Load factor: "<<static_cast<double>(uniqueWordCount)/
            static_cast<double>(MAX)<<"\n";
    cout<<"\n";
}

//reading input file
int readFile(string FILE_NAME,const int MAX,const unsigned MIN_WORD,NODE* table[])
{
    ifstream inFile;
    string inWord;
    string resultWord;
    unsigned int index;
    int collisions=0;
    cout<<"Project done by ZOE ZHOU\n\nOpening file ulyss12.txt\n\n";
    inFile.open(FILE_NAME);

    if(inFile.fail())
    {
        cout<<" File failure..exiting program...";
        cin.get();
        return 0;
    }
    while(inFile >> inWord)
    {
        resultWord = preProcess(inWord,MIN_WORD);//processing the word
        if(resultWord.length() > MIN_WORD)
        {
            index = hashFunction(resultWord,MAX);//hashing the word and
                                            //finding the key value
            collisions +=store(index,table,resultWord);//storing the word at
                        //the key generated and summing the collisions occurred
        }
    }
    return collisions;
}
string preProcess(string word,const unsigned MIN_WORD)//changing capital letters to
                                // lowercase and removing any quotation marks
                                //and eliminating numbers
{
    string temp;
    unsigned pos=0;
    bool num = false;
    if(word.length() <= MIN_WORD) //if word length is less than 5
    {
        return temp;
    }
    while(word.length() > MIN_WORD && (!isalpha(word.at(pos))))
    {
        word.erase(pos,1);
    }
    if(word.length()>MIN_WORD)
    {
        pos = word.length() - 1;
        while(pos>=0 && (!isalpha(word.at(pos))))
        {
            word.erase(pos,1);
            pos--;
        }
    }
    if(word.length() > MIN_WORD)
    {
        for(unsigned i=0; i<word.length(); i++)
        {
            if(isdigit(word.at(i)))
            {
                num = true;
            }
        }
        if(num)
        {
            return temp;
        }
    }
    if(word.length() > MIN_WORD)
    {
        for(unsigned i=0; i<word.length(); i++)
        {
            word.at(i) = tolower(word.at(i));
        }
        return word;
    }
    return temp;
}

unsigned int hashFunction(string word,int tableSize)//the hash function
                                        //taken from class notes
{
    unsigned seed = 1313;
    unsigned int key = 0;
    for(unsigned i =0; i<word.length(); i++)
    {
        key = (key*seed)+ word[i];
    }
    key = key%tableSize;
    return key;
}
int store(unsigned key,NODE* table[],string word)//storing word in hash table
{
    NODE* newNode = new NODE();//creating the node to insert word and count
    newNode->word = word;
    newNode->count=1;
    newNode->next = nullptr;
    int flag=1;
    int collisions=0;
    if(table[key] == nullptr)//if table at position=key is null then add newnode
    {
        table[key] = newNode;
    }
    else
    {
        if(table[key]->word  == word) //if the value @table[key]
                                        //is the word send then increase count
        {
            table[key]->count++;
            return 0;
        }
        NODE* previous_ptr = nullptr;
        NODE* ptr = table[key];

        bool insert = true;
        while( ptr != nullptr)// if the value table[key]
                //is the different word then traverse the linkedlist at that key
        {

            if(ptr->word == word)//if the word is present in the linkedlist
                                //then increment the count
            {
                ptr->count++;
                insert = false;
                flag=0;
                break;
            }
            collisions++;
            previous_ptr = ptr;
            ptr=ptr->next;
        }
        if(flag==0){
            return 0;
        }
        if (insert == true)
            previous_ptr->next = newNode; //if the word is not present then
                                        // add the newnode to the linkedlist
    }
    return collisions;
}

