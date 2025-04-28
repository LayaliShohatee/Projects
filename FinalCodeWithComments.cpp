
// Summer Final Project

#include <iostream>
#include <vector>
#include <string>

namespace Summer
{


class CPU 
{
private:
    int accumulator; 
    std::vector<int> memory; // stores a series of ints 
    
    bool equalFlag;  // they indicate if the accumulator is equal to, less than, or greater than a value in memory.
    bool lessThanFlag;
    bool greaterThanFlag;

    bool isValidAddress(int memoryAddress) // checks if the memory address it was given is valid
	{
        return (memoryAddress >= 0 && memoryAddress < memory.size()); // returns trie os memoryAddress is within valid range
    }

public:
    CPU(int memorySize) : accumulator(0), equalFlag(false), lessThanFlag(false), greaterThanFlag(false) // constructor
	{
        memory.resize(memorySize, 0);
    }

    void load(int memoryAddress)  // loads a value from the memory address into the accumulator
	{
        if (isValidAddress(memoryAddress)) 
            accumulator = memory[memoryAddress]; // (Container) brackets bc its used to access elements within the memory vector at a specific index
        
		else 
            std::cout << "Not a valid memory address." ; // checks if its valid otherwise, prints error message
        
    }

    void store(int memoryAddress) // stores a value from the memory address into the accumulator
	{
        if (isValidAddress(memoryAddress)) 
            memory[memoryAddress] = accumulator;
        
		else 
            std::cout << "Not a valid memory address." ;
        
    }

    void add(int memoryAddress) // adds the value from a memory address into the accumulator
	{
        if (isValidAddress(memoryAddress)) 
            accumulator += memory[memoryAddress];
        
		else 
            std::cout << "Not a valid memory address." ;
    }

    void subtract(int memoryAddress) // subtracts the value from a memory address into the accumulator
	{
        if (isValidAddress(memoryAddress)) 
            accumulator -= memory[memoryAddress];
        else 
            std::cout << "Not a valid memory address." ;
        
    }

    void multiply(int memoryAddress)   // multiplies the value from a memory address into the accumulator
	{
        if (isValidAddress(memoryAddress)) 
            accumulator *= memory[memoryAddress];
        
		else 
            std::cout << "Not a valid memory address." ;
        
    }

    void divide(int memoryAddress) // divides the accumulator by the value at a memory address
	{
        if (isValidAddress(memoryAddress))
		{
            if (memory[memoryAddress] != 0) 
                accumulator /= memory[memoryAddress];
             else 
                std::cout << "Cannot divide by zero." ;
            
        } 
		
		else
            std::cout << "Not a valid memory address." ;
    
    }

        void compare(int memoryAddress) // compares the value at a memory address with the accumulator
		{
        if (isValidAddress(memoryAddress)) 
		{
            int value = memory[memoryAddress];
            
            if (accumulator == value) 
			{
                equalFlag = true; // sets equal to true and if it is the rest are set to false
                lessThanFlag = false;
                greaterThanFlag = false;
            }
			 else if (accumulator < value) 
			 {
                equalFlag = false;
                lessThanFlag = true; // sets the flag in the accumulator to true if the value is less than
                greaterThanFlag = false;
            } 
			else 
			{
                equalFlag = false;
                lessThanFlag = false;
                greaterThanFlag = true;
            }
        } 
		else 
		    std::cout << "Not a valid memory address." ;
            }

    void logicalAnd(int memoryAddress) // Performs a logical AND operation between the accumulator and the value at the specified memory address.
	{
        if (isValidAddress(memoryAddress)) 
		  accumulator = accumulator && memory[memoryAddress];
        
		else 
            std::cout << "Not a valid memory address." ;
        
    }

    void logicalOr(int memoryAddress) // same thing with OR
	{
        if (isValidAddress(memoryAddress)) 
            accumulator = accumulator || memory[memoryAddress];
        
		else 
            std::cout << "Not a valid memory address." ;
        
    }

    void logicalNot() {  accumulator = !accumulator;    } // same thing with NOT

    int getAccumulator()  {   return accumulator;    } // Returns the current value of the accumulator.
    
};

template <typename T> // template class definiton
// T  represents a placeholder for a type that will be specified when the template is used

class Value 
{
private:
    T value; // priv variable type, where the value stored by the class is held
public:
    Value() : value(0) {} // default constructor 

    Value(T val) : value(val) {} // non default constructor

    Value operator+(T other)  {  return Value(value + other);    } // Overloads the + operator to perform addition with another T type value

    Value operator-(T other)  {    return Value(value - other);    } // overloads the - operator to perform subrtaction

    Value operator*(T other)  {    return Value(value * other);    } // overloads the * operator to perform multiplication

    Value operator/(T other)  // Overloads the / operator to perform division.  
	{
        if (other != 0)  // checks to prevent division by zero, which outputs an error message and exits the program
            return Value(value / other);
        
		else
		 {
            std::cout << "Cannot divide by zero." ;
            exit(1);
        }
    }

    Value& operator+=( Value& other) //overloads to add the value of another value T to the current template
	{
        value += other.value;
        return *this;
    }

    Value& operator-=( Value& other) // overloads for subtraction
	{
        value -= other.value;
        return *this;
    }

    Value& operator*=( Value& other) // over;pads for multiplication
	{
        value *= other.value;
        return *this;
    }

    Value& operator/=( Value& other) // overloads for division 
	{
        if (other.value != 0)  // checks for division by zero
		{
            value /= other.value;
            return *this;
        } 
		else 
		{
            std::cout << "Cannot divide by zero." ;
            exit(1);
        }
    }

    T getValue()  {      return value;   } //Accesses the current value stored in the template 7 Returns the current value of the template.
    
};

class IceCream : public Value<int> //uses the template via inheretance 
{
public:
    IceCream() : Value<int>(0) {} // default constructor 

    IceCream(int val) : Value<int>(val) {} // takes an int argument called value and initializes it with value
};


class Beach : public Value<double> // inherits from the template specifically for a double
{
public:
    Beach() : Value<double>(0.0) {} // default constructor 

    Beach(double val) : Value<double>(val) {} //initializes value with the double argument val
};


class Cake : public Value<char> //inherits from the template specifically for a char
{
public:
    Cake() : Value<char>(0) {} // default constructor 

    Cake(char val) : Value<char>(val) {} // initialized value with the char argument val
};


class Sun : public Value<std::string>  // inherits from the template specifically for a string
{
public:
    Sun() : Value<std::string>("") {} // default constructor

    Sun( std::string& val) : Value<std::string>(val) {} //  initializes value with the string argument val.
};


class Sunrise // facilitates custom output operations using the ^ operator.
{ // start chat gpt code
private:
    std::ostream& out;

public:
    Sunrise(std::ostream& os = std::cout) : out(os) {} // the construcor initializes out with the std::ostream object

    template<typename T>
    Sunrise& operator^(T& value) // outputs value to out
	{
        out << value;
        return *this;
    }

    template<typename T>
    Sunrise& operator^(T&& value)  // Outputs value to out.

	{
        out << std::forward<T>(value);
        return *this;
    }

    Sunrise sunrise(std::ostream& os = std::cout)  // create a Sunrise object with a specified std::ostream.
	{
        return Sunrise(os);
    }
};   // end chat gpt code


class Sunset // creating custom input operations using the ^ operator.
{
private:
    std::istream& in;

public:
    Sunset(std::istream& is = std::cin) : in(is) {} //Constructor initializes in with the provided std::istream object

    template<typename T>
    Sunset& operator^(T& value)  // Reads input into value from in.
	{
        in >> value;
        return *this;
    }

    template<typename T>
    Sunset& operator^(T&& value) //Reads input into value from in.
	{
        in >> std::forward<T>(value);
        return *this;
    }

 	Sunset sunset(std::istream& is = std::cin) {   return Sunset(is); } // create a Sunset object with a specified std::istream
    
};

class Memory // manages a block of memory using a vector
{
private:
    std::vector<int> memory;

public: 
    Memory(int size) : memory(size, 0) {} // initializes memory vector with size elements, all initialized to 0.

    void set(int address, int value)  // Sets the value at a specific address in memory.
	{
        if(address >= 0 && address < memory.size()) 
            memory[address] = value; // Checks if address is valid; if not, prints an error message and exits.
        
		else 
		{ std::cout << "Can't access memory, try again." ; // else prints and error message
            exit(1);
        }
    }

    int get(int address)  
	{
        if(address >= 0 && address < memory.size()) //  Retrieves the value stored at a specific address in memory.
            return memory[address];
    	else 
		{
            std::cout << "Can't access memory, try again."; // else prints and error message
            exit(1);
        }
    }
};

}

using namespace Summer;

int main()
{
	return 0;
}

