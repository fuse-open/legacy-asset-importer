/* * Copyright (C) 2010-2014 Outracks Technologies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __XLI_QUEUE_H__
#define __XLI_QUEUE_H__

#include "Array.h"

namespace Xli
{
    /**
        \ingroup XliContainers
    */
    template <typename T> class Queue
    {
        struct Node
        {
            Node* Next;
            T Value;
        };

        Node* head;
        Node* tail;

        Array<Node*> freeNodes;
        int len;

        Queue(const Queue& copy);
        Queue& operator = (const Queue& copy);

    public:
        Queue()
        {
            head = 0;
            tail = 0;
            len = 0;
        }

        ~Queue()
        {
            Clear();
            Trim();
        }

        void Trim()
        {
            for (int i = 0; i < freeNodes.Length(); i++)
                delete freeNodes[i];

            freeNodes.Clear();
            freeNodes.Trim();
        }

        void Clear()
        {
            while (head)
            {
                Node* temp = head;
                head = head->Next;
                freeNodes.Add(temp);
            }

            tail = 0;
            len = 0;
        }

        void Enqueue(const T& value)
        {
            Node* n = freeNodes.Length() ? freeNodes.RemoveLast() : new Node();

            n->Next = 0;
            n->Value = value;

            if (tail)
            {
                tail->Next = n;
                tail = n;
            }
            else
            {
                head = n;
                tail = n;
            }

            len++;
        }

        const T& Peek() const
        {
#ifdef XLI_RANGE_CHECK
            if (!head) 
                XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif
            return head->Value;
        }

        T Dequeue()
        {
#ifdef XLI_RANGE_CHECK
            if (!head) 
                XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif
            Node* oldHead = head;
            head = head->Next;
            if (!head) tail = 0;
            T value = oldHead->Value;
            freeNodes.Add(oldHead);
            len--;
            return value;
        }

        int Count() const
        {
            return len;
        }

        bool IsEmpty() const
        {
            return len == 0;
        }
    };
}


#endif
