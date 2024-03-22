#pragma once
#include <vector>
#include <functional>
#include <iostream>
#include <utility>
#include <type_traits>

namespace AG
{
	//template that accepts multiple types as arguments
	template<typename... T>
	struct Binding
	{
	protected:
		//Store hash values
		size_t hash;

		//Create listen function
		std::function<void(T...)> listener;
	public:
		//Constructor of Binding Struct - takes func as a parameter, which represents the listener function associated with the binding.
		//retrieves the type information of the target of the func function object and computes its hash code. This hash code is stored in the hash member variable of the Binding.
		//moves func to listener variable, transferring ownership of the function object to the Binding.
		Binding(std::function<void(T...)> func) : hash(func.target_type().hash_code()), listener(std::move(func)) {}

		//Compares the hash member of the current Binding object to the hash member of another Binding object (rhs). True if both hash values are the same
		bool operator==(const Binding<T...>& rhs) { return hash == rhs.hash; }

		//Compares the hash member of the current Binding object to the hash member of another Binding object (rhs). True if both hash values are not the same
		bool operator!=(const Binding<T...>& rhs) { return hash != rhs.hash; }

		//Returns the hash code asscoiated with the Binding
		constexpr size_t hash_code() const throw() { return hash; }

		//Invokes the listener function associated with the Binding - takes args as an argument and forwards them to the listener function using perfect forwarding.
		//After invoking the listener function, it returns a reference to the current Binding object.
		Binding<T...>& Invoke(T... args) { listener(static_cast<T&&>(args)...); return (*this); }

		//Allows instances of the Binding struct to be called as if they were functions.
		//Forwards 'args' to the listener function associated with the Binding using perfect forwarding.
		void operator()(T... args) { listener(static_cast<T&&>(args)...); }
	};



	template<typename... T>
	struct Event
	{
	private:
		//A vector that can store multiple instances of the Binding struct, each capable of holding a listener function with a different set of parameter types.
		std::vector<Binding<T...>> listeners;

	public:

		//Invokes all listener functions stored in the listeners vector.
		//Takes a variable number of arguments (args) and forwards them to each listener function
		//Iterates through each Binding object in the listeners vector and calls the Invoke function of each Binding to execute the associated listener function
		//Returns a reference to the current Event object.
		Event<T...>& Invoke(T... args) { for (Binding<T...> l : listeners) l.Invoke(static_cast<T&&>(args)...); return (*this); }

		//Adds a listener to the listeners vector if it doesn't already exist.
		void AddListener(const Binding<T...> listener)
		{
			//Uses std::find_if to check if a Binding object with the same hash code as the provided listener exists in the listeners vector. If not, it adds the listener to the vector.
			if (std::find_if(listeners.begin(), listeners.end(), [listener](Binding<T...> b) {return listener.hash_code() == b.hash_code(); }) == listeners.end())
			{
				listeners.push_back(listener);
			}
		}

		//Removes a listener from the listeners vector using std::erase_if
		void RemoveListener(const Binding<T...> listener) { std::erase_if(listeners, [listener](Binding<T...> b) {return listener.hash_code() == b.hash_code(); }); }

		//Clears all listeners from the listeners vector by calling clear() 
		void Empty() { listeners.clear(); }




		///////////////////These operators provide a way to add and remove listeners from the Event object///////////////////


		//Allows instances of the Event class to be called as if they were functions.
		//When called it forwards these arguments to the Invoke function, which invokes all the listener functions with the provided arguments.
		//Returns a reference to the current Event object
		Event<T...>& operator()(T... args) { Invoke(args...); return (*this); }


		//Takes a Binding object (listener) as a parameter and adds it to the listeners vector using the AddListener member function.
		//After adding the listener, it returns a reference to the current Event object
		Event<T...>& operator+=(const Binding<T...> listener) { AddListener(listener); return (*this); }


		//Creates a Binding object (b) from the provided std::function<void(T...)> object (func)
		//Uses operator+= to add the listener to the listeners vector, then it returns a reference to the current Event object	
		Event<T...>& operator+=(const std::function<void(T...)> func) { Binding<T...> b(func); *this += b; return (*this); }


		//Takes a Binding object (listener) as a parameter and removes it from the listeners vector using the RemoveListener member function.
		//After removing the listener, returns a reference to the current Event
		Event<T...>& operator-=(const Binding<T...> listener) { RemoveListener(listener); return (*this); }


		//Creates a Binding object (b) from the provided std::function<void(T...)> object (func).
		//Uses operator-= to remove the listener to the listeners vector, then it returns a reference to the current Event object	
		Event<T...>& operator-=(const std::function<void(T...)> func) { Binding<T...> b(func); *this -= b; return (*this); }
	};
}
