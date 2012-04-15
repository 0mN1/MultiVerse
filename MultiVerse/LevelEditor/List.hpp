#ifndef __LIST_HPP__
#define __LIST_HPP__

#include <cstdlib>

namespace LIST
{
    // Nod-klass
    template <class T>
    class Node
    {
        protected:
        T * Item; // inneh�ll

        public:
        Node * Next; // Pekare till n�sta nod
        Node * Prev; // Pekare till tidigare nod

        Node(Node * Parent = NULL) // Konstruktor
        {
            Prev = Parent;
            Next = NULL;

            Item = new T;
        }

        /*~Node() // Destruktor
        {
            delete Item;

            if(Next) // Om det finns ytterliggare en nod
            delete Next; // Ta d� bort den
        }*/

        T * GetItem() // Returnerar nodens inneh�ll
        {
            return Item;
        }

        void SetItem(T _item) // S�tter nodens inneh�ll
        {
            *Item = _item;
        }

        void SetItem(T * _item) // S�tter pekaren till nodens inneh�ll
        {
            Item = _item;
        }
    };

    // List-klass
    template <class T>
    class List
    {
        protected:
        int Size; // Storlek p� listan
        Node<T> * FirstNode; // Pekare till f�rsta noden

        public:
        List() // Konstruktor
        {
            Size = 0; // Storleken �r noll i b�rjan
            FirstNode = NULL; // Det finns ingen nod �n
        }

        List(const List< T > & Copy) // Kopierings-Konstruktor
        {
            Size = Copy.Size;
            FirstNode = NULL;

            if(Size > 0)
            {
                FirstNode = new Node<T>;
                FirstNode->SetItem(*Copy.FirstNode->GetItem());

                Node<T> * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

                for(int i = 1; i < Copy.Size; i++)
                {
                    CopyCurrent = CopyCurrent->Next;

                    NewNode = new Node<T>;
                    NewNode->SetItem(*CopyCurrent->GetItem());

                    Current->Next = NewNode;

                    Current = NewNode;
                }
            }
        }

        ~List() // Destruktor
        {
//            if(FirstNode) // Om listan inneh�ller n�got
//            delete FirstNode; // Ta bort allt inneh�ll
            Clear();
        }

        void Clear() // Rensar listan
        {
            int size = Size;
            for(int i = 0; i < size; i++)
            RemoveNode(0);
        }

        int GetSize() // Returnerar listans storlek
        {
            return Size;
        }

        int GetLenght() // Returnerar listans storlek
        {
            return Size;
        }

        void AddNode(T _item) // L�gg till en nod
        {
            if(FirstNode) // Om listan inneh�ller n�got
            {
                Node<T> * Current = FirstNode; // Peka Current mot den f�rsta noden

                while(Current->Next) // S� l�nge det finns en till nod
                {
                    Current = Current->Next; // Peka Current mot n�sta nod
                }

                Current->Next = new Node<T>(Current); // Skapa en ny nod
                Current->Next->SetItem(_item); // och ge den sitt inneh�ll

                ++Size; // �ka listans storlek med ett
            }
            else // Om listan inte inneh�ller n�got
            {
                FirstNode = new Node<T>(); // Skapa den f�rsta noden
                FirstNode->SetItem(_item); // och ge den sitt inneh�ll

                ++Size; // �ka listans storlek med ett
            }
        }

        void AddNode(T * _item) // L�gg till en nod
        {
            if(FirstNode) // Om listan inneh�ller n�got
            {
                Node<T> * Current = FirstNode; // Peka Current mot den f�rsta noden

                while(Current->Next) // S� l�nge det finns en till nod
                {
                    Current = Current->Next; // Peka Current mot n�sta nod
                }

                Current->Next = new Node<T>(Current); // Skapa en ny nod
                Current->Next->SetItem(_item); // och ge den sitt inneh�ll

                ++Size; // �ka listans storlek med ett
            }
            else // Om listan inte inneh�ller n�got
            {
                FirstNode = new Node<T>(); // Skapa den f�rsta noden
                FirstNode->SetItem(_item); // och ge den sitt inneh�ll

                ++Size; // �ka listans storlek med ett
            }
        }

        void RemoveNode(int n) // Ta bort en nod
        {
            if(n >= 0 && n < Size) // Om �nskad nod finns i listan
            {
                Node<T> * Current = FirstNode; // Peka Current mot den f�rsta noden

                for(int i = 0; i < n; i++)
                {
                    Current = Current->Next; // R�kna fram till den noden som ska tas bort
                }

                if(Current->Prev) // Om det inte �r den f�rsta noden
                Current->Prev->Next = Current->Next; // Ge noden innan en pekare till noden efter
                else // Annars
                FirstNode = Current->Next; // S�tt f�rsta noden till n�sta nod

                if(Current->Next) // Om det inte �r den sista noden
                Current->Next->Prev = Current->Prev; // Ge noden efter en pekare till noden innan

                Current->Next = NULL; // S�tt pekarna till NULL
                Current->Prev = NULL; // s� att dom inte tas bort av konstruktorn

                delete Current; // Ta bort noden

                --Size; // och minska storleken p� listan med ett
            }
        }

        void SwitchNodes(int n1, int n2)
        {
            if(n1 >= 0 && n1 < Size
            && n2 >= 0 && n2 < Size)
            {
                Node<T> * Current1 = FirstNode;

                for(int i = 0; i < n1; i++)
                {
                    Current1 = Current1->Next;
                }

                Node<T> * Current2 = FirstNode;

                for(int i = 0; i < n2; i++)
                {
                    Current2 = Current2->Next;
                }

                T * Temp = Current1->GetItem();
                Current1->SetItem(Current2->GetItem());
                Current2->SetItem(Temp);
            }
        };

        T * operator[](int n) // Returnerar inneh�llet av �nskad nod i listan
        {
            if(n >= 0 && n < Size) // Om �nskad nod finns i listan
            {
                Node<T> * Current = FirstNode; // Peka Current mot den f�rsta noden

                for(int i = 0; i < n; i++)
                {
                    Current = Current->Next; // R�kna fram till den noden som ska tas bort
                }

                return Current->GetItem(); // och returnera nodens inneh�ll
            }

            return NULL; // Returnera noll
        }

        T * GetElement(int n) // Returnerar inneh�llet av �nskad nod i listan
        {
            if(n >= 0 && n < Size) // Om �nskad nod finns i listan
            {
                Node<T> * Current = FirstNode; // Peka Current mot den f�rsta noden

                for(int i = 0; i < n; i++)
                {
                    Current = Current->Next; // R�kna fram till den noden som ska tas bort
                }

                return Current->GetItem(); // och returnera nodens inneh�ll
            }

            return NULL; // Returnera noll
        }

        void Reverse()
        {
            for(int i = 0; i < Size / 2; i++)
            {
                SwitchNodes(i, Size-1 - i);
            }
        }
    };
}

#endif
