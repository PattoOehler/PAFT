#ifndef LINKED_LIST
#define LINKED_LIST


struct node
{
    std::string data;
    node *next;
};

class Linked_List
{
public:
    node *head,*tail;
    int length;

    Linked_List()
    {
        head = NULL;
        tail = NULL;
        length = 0;
    }

    void Add_Node(std::string n)
    {
        node *tmp = new node;
        tmp->data = n;
        tmp->next = NULL;

        if(head == NULL)
        {
            head = tmp;
            tail = tmp;
        }
        else
        {
            tail->next = tmp;
            tail = tail->next;
        }
        length ++;
    }

    std::string Pop_Head()
    {
        if(head==NULL)
            return "";

        std::string data = head->data;

        head = head->next;


        length --;
        return data;


    }



};


#endif // LINKED_LIST
