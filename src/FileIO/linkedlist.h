#ifndef LINKEDLIST
#define LINKEDLIST


struct node
{
    std::string data;
    node *next;
};

class linked_list
{
public:
    node *head,*tail;
    int length;

    linked_list()
    {
        head = NULL;
        tail = NULL;
        length = 0;
    }

    void add_node(std::string n)
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

    std::string pop_head()
    {
        if(head==NULL)
            return "";

        std::string data = head->data;

        head = head->next;


        length --;
        return data;


    }



};


#endif // LINKEDLIST
