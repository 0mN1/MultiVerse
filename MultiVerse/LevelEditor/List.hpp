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
        T * Item; // innehåll

        public:
        Node * Next; // Pekare till nästa nod
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
            delete Next; // Ta då bort den
        }*/

        T * GetItem() // Returnerar nodens innehåll
        {
            return Item;
        }

        void SetItem(T _item) // Sätter nodens innehåll
        {
            *Item = _item;
        }

        void SetItem(T * _item) // Sätter pekaren till nodens innehåll
        {
            Item = _item;
        }
    };

    // List-klass
    template <class T>
    class List
    {
        protected:
        int Size; // Storlek på listan
        Node<T> * FirstNode; // Pekare till första noden

        public:
        List() // Konstruktor
        {
            Size = 0; // Storleken är noll i början
            FirstNode = NULL; // Det finns ingen nod än
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
//            if(FirstNode) // Om listan innehåller något
//            delete FirstNode; // Ta bort allt innehåll
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

        void AddNode(T _item) // Lägg till en nod
        {
            if(FirstNode) // Om listan innehåller något
            {
                Node<T> * Current = FirstNode; // Peka Current mot den första noden

                while(Current->Next) // Så länge det finns en till nod
                {
                    Current = Current->Next; // Peka Current mot nästa nod
                }

                Current->Next = new Node<T>(Current); // Skapa en ny nod
                Current->Next->SetItem(_item); // och ge den sitt innehåll

                ++Size; // Öka listans storlek med ett
            }
            else // Om listan inte innehåller något
            {
                FirstNode = new Node<T>(); // Skapa den första noden
                FirstNode->SetItem(_item); // och ge den sitt innehåll

                ++Size; // Öka listans storlek med ett
            }
        }

        void AddNode(T * _item) // Lägg till en nod
        {
            if(FirstNode) // Om listan innehåller något
            {
                Node<T> * Current = FirstNode; // Peka Current mot den första noden

                while(Current->Next) // Så länge det finns en till nod
                {
                    Current = Current->Next; // Peka Current mot nästa nod
                }

                Current->Next = new Node<T>(Current); // Skapa en ny nod
                Current->Next->SetItem(_item); // och ge den sitt innehåll

                ++Size; // Öka listans storlek med ett
            }
            else // Om listan inte innehåller något
            {
                FirstNode = new Node<T>(); // Skapa den första noden
                FirstNode->SetItem(_item); // och ge den sitt innehåll

                ++Size; // Öka listans storlek med ett
            }
        }

        void RemoveNode(int n) // Ta bort en nod
        {
            if(n >= 0 && n < Size) // Om önskad nod finns i listan
            {
                Node<T> * Current = FirstNode; // Peka Current mot den första noden

                for(int i = 0; i < n; i++)
                {
                    Current = Current->Next; // Räkna fram till den noden som ska tas bort
                }

                if(Current->Prev) // Om det inte är den första noden
                Current->Prev->Next = Current->Next; // Ge noden innan en pekare till noden efter
                else // Annars
                FirstNode = Current->Next; // Sätt första noden till nästa nod

                if(Current->Next) // Om det inte är den sista noden
                Current->Next->Prev = Current->Prev; // Ge noden efter en pekare till noden innan

                Current->Next = NULL; // Sätt pekarna till NULL
                Current->Prev = NULL; // så att dom inte tas bort av konstruktorn

                delete Current; // Ta bort noden

                --Size; // och minska storleken på listan med ett
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

        T * operator[](int n) // Returnerar innehållet av önskad nod i listan
        {
            if(n >= 0 && n < Size) // Om önskad nod finns i listan
            {
                Node<T> * Current = FirstNode; // Peka Current mot den första noden

                for(int i = 0; i < n; i++)
                {
                    Current = Current->Next; // Räkna fram till den noden som ska tas bort
                }

                return Current->GetItem(); // och returnera nodens innehåll
            }

            return NULL; // Returnera noll
        }

        T * GetElement(int n) // Returnerar innehållet av önskad nod i listan
        {
            if(n >= 0 && n < Size) // Om önskad nod finns i listan
            {
                Node<T> * Current = FirstNode; // Peka Current mot den första noden

                for(int i = 0; i < n; i++)
                {
                    Current = Current->Next; // Räkna fram till den noden som ska tas bort
                }

                return Current->GetItem(); // och returnera nodens innehåll
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
