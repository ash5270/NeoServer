#pragma once

#include<iostream>
#include<atomic>
#include<memory>
using namespace std;


namespace neo::util::system
{
	template<class T> 
	class LockFreeQueue
	{
	public:
		LockFreeQueue()
		{
			Node* node = new Node();
			mHead.store(node);
			mTail.store(node);
			
		}

		~LockFreeQueue()
		{
			T output;
			
			delete mHead.load();
		}

		void Enqueue(T item)
		{
			Node* node = new Node();
			node->Value = std::move(item);
			//메모리 접근 방식 느슨하게
			Node* tail = mTail.load(std::memory_order_relaxed);
			Node* next = nullptr;
			
			//tail의 next가 nullptr이 아니면 
			//tail이 다른 스레드에 의해 업데이트되고 있으므로,
			//자신도 업데이트를 시도함
			
			while (true)
			{
				//여기서 tail의 다음 거를 가지고 옴
				//tail의 값을 가져온다.
				//tail nullptr이면 현재 마지막 queue last back라는 의미이다.
				next = tail->Next.load(std::memory_order_acquire);
				if (!next)
				{
					if (tail->Next.compare_exchange_strong(next, node, std::memory_order_relaxed))
						break;
				}
				//tail nullptr이 아니라면 
				//어디 쓰레드에서 값을 쓰고 있다는 소리로 내 자신도 tail 값을 업데이트 시켜야한다.
				else
				{
					//tail과 mtail의 값이 같다며 next 값을 넣어줌
					mTail.compare_exchange_strong(tail, next);
				}
			}
			//마지막으로 queue 꼬리 부분을 바꿔준다.
			mTail.compare_exchange_strong(tail, node);
		}

		bool Dequeue(T& output)
		{
			Node* head = mHead.load(std::memory_order_relaxed);
			Node* tail = mTail.load(std::memory_order_relaxed);
			Node* next = head->Next.load(std::memory_order_acquire);


			//비어있음
			if (head == tail && next == nullptr)
				return false;

			//head와 tail이 같은 경우
			if (head == tail)
			{
				tail = mTail.load(std::memory_order_acquire);
				next = head->Next.load(std::memory_order_acquire);
				if (next == nullptr)
					return false;

				mTail.compare_exchange_strong(tail, next);
			}

			//head 업데이트
			mHead = next;
			output = std::move(next->Value);

			delete head;
			return true;
		}



	private:
		struct Node {
			Node() : Next(nullptr){}
			Node(const T& value):Value(value), Next(nullptr){}
			
			T Value;
			std::atomic<Node*> Next;
		};

		std::atomic<Node*> mHead;
		std::atomic<Node*> mTail;
		
	};
}
