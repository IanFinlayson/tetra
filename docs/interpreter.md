#####This document is currently a work in progress.

This document contains the public interfaces of all classes in the interpreter. This document presumes that you are already somewhat familiar with the Tetra frontend, and the Tetra language.
For clarity, whenever this documentation uses "the program", it refers to the tetra program being executed by the interpreter. "The interpreter" refers to some action or attribute of the tetra interpreter.

###TetraEnvironment

	The TetraEnvironment class contains static methods dealing with some of the particulars  of where the program is being executed. Its methods are:

```cpp
static void initialize()
static void initialize(const ConsoleArray&)
```

sets up the environment with default values. Any program utilizing the interpreter should call this method. The second version also sets the given ConsoleArray for the environment (For more on Consoles, see the ConsoleArray and VirtualConsole documentation).

```cpp
static void setConsoleArray(const ConsoleArray&)
static const ConsoleArray& getConsoleArray()
```

These methods explicitely get and set the ConsoleArray for the TetraEnvironment.

```cpp
static const VirtualConsole& getConsole(int, bool)
```

This method gets a specific console according to the policy of the set ConsoleArray.

```cpp
static void setObserver(const VirtualObserver&)
static VirtualObserver& getObserver()
```

Gets and sets the observer for the TetraEnvironment (for more on observers, see the VirtualObserver documentation).

These methods explicitely get and set the console for the TetraEnvironment.
```cpp
static int getMaxThreads()
static void setMaxThreads(int)
```

gets and sets the number of threads that should be started when a parallel for-loop is initialized.

```cpp
static void setDebug(bool)
static bool isDebugMode()
```

These methods are used to set and query whether or not the interpreter is set to keep track of debug information, and notify the debugger during execution.

```cpp
static std::string parseFlags(std::string*,int)
```

This method takes an array of flags (represented as strings) and the length of the arrays. It essentially takes raw command-line arameters, and makes the requested changes to the interpreter. (as of 4/24/15, the only flag supported is debugging). If there is an error parsing, a message is returned. Otherwise, the method returns "". 

```cpp
static int obtainNewThreadID();
```

Each time this method is called, a new integer is returned (0, then 1, then 2, etc) in a threadsafe manner. Used to assign numeric IDs to threads.


###TData<T>
	The TData class is a templated class used to handle all data for the program. In pracice, T may be int, double, bool, string, TArray, pointers to any of the preceding types, and void*. Note that although a TData<T> may contain a pointer to dynamically allocated memory, the TData<T> will neither allocate nor deallocate the memory unless its deletable type has been set (see setDeletableType<T>()).

	To construct a TData<T>, you can use either the default or copy constructor, or use

```cpp
TData<T>(T pData)
```

to initialize th object with the value pData.

Getting the data can be accomplished through the method

```cpp
T getData() const
```

Setting the data can be accomplished using the templated function

```cpp
template<typename R>
bool setData<R>(const R otherData)
```

The function returns true if the assignment was successful, false otherwise. The reason for this seemingly awkward definition has to do partly with how the interpreter is designed, and also to allow future expansion of interpreter functionality (i.e. if one day we define how to assign a bool to a string, we can simply define a specialized template for TData<string>::setData<bool>(...)). Note that if T (the type of the TData) is "void*", then the user must explicitely use the generics in the call "obj.setData<void*>(ptr)", else the assignment will not actually take place.

To assist TData users with cleaning up, the TData<void*> class provides uses the functions

```cpp
const DataType getPointedTo()

template<typename R>
void setDeletableType<R>()
```

This is used to keep track of type information of void* pointers. By default, TData objects getPointedTo() returns TYPE_VOID. To change this, pass the appropriate data type as the template argument to setDeletableType().

	int = TYPE_INT
	double = TYPE_REAL
	bool = TYPE_BOOL
	string = TYPE_STRING
	TArray = TYPE_VECTOR

	Any other type will revert it to TYPE_VOID.
Once a TData's deletable type has been set, it will result in a deep copy if used as the argument of a copy constructor or copy assignment. If the deletable type is set, a TData<void*> will cast its pointer to the specified type, and free it. Note that for referenced types(e.g. TArray), a smart pointer is deleted, meaning the underlying data is untouched uless the last reference was destroyed.

###TArray

	The TArray class wraps references to vectors of TData<void*>. They simulate arrays in a tetra program. A TArray actually wraps a smart pointer to a vector. This means that copy assignment results in a shallow copy. For the interpreter, this is desired behavior.
	The methods are fairly self explanatory.

```cpp
void addElement(const TData<void*>&)
```

This method has some prerequisites. The void* contained in the argument must already be pointing to the correct value. Furthermore, it should have had its setDeletableType() method called with the appropriate type.

To access an element, use

```cpp
TData<void*>& elementAt(unsigned int)
```

This method comes in a const and non-const variety. Note that this method returns a reference, so any changes done to the returned value will be done to the original array. Since the returned TData is what is actually in the TArray, it will have a set deletable type (assuming it was added with one), and so will be deep copied if a copy is made.

The TArray class also provides methods

```cpp
const std::vector< TData<void*> >::const_iterator begin() const
const std::vector< TData<void*> >::const_iterator end() const
```

to get iterators to the underlying container.

The class also defines a friend method

```cpp
friend std::ostream& operator<<(std::ostream&, TArray)
```

which is used to print the contents of a TArray.

The class also contains many stubbed operators for type TArray. At present, all logical operators return false, and all other operators return the first operand.

###VarTable

As mentioned above, each scope has a VarTable. The VarTable class contains two methods: lookupVar and containsVar. The interface for a VarTable is identical to that of a TetraScope (the TetraScope interface merely calls the same method as the VarTable it contains). Refer to that class for documentation on what those methods do. The VarTable, in addition to some minor housekeeping for threading, contains an instance of the VarHash class to manage data storage.

###VarHash

The VarHash class merely provides a bare-bones interface to store pointers to data values for a given scope. Presently, the interface facilitates only mild inconvenience between switching between this implementation, and stl data structures. It is constructed via

```cpp
VarHash(int size)
```
where size specifies the number of entries the table can hold. As of 1/20/15 however, the size will be 30 regardless of the specified size.

```cpp
TData<void*>& operator[](int index)
```

This method returns a reference to the pointer specified by index. Note that all the pointers contained within the VarHash are initialized to NULL, so make sure that you have initialized the pointer before using it.

```cpp
bool exists(const Node* varNode) const
```

This method checks whether or not the variable (i.e. pointer to a variable) denoted by the given node is a NULL pointer (returns false), or if it has some non-NULL value (returns true).

###TetraScope

The TetraScope class contains information about the current scope of the program. This class maintains a VarTable to keep track of local variables, and an execution flag which signals what the interpreter should do when it encounters certain structual nodes within the program.

To lookup local variables in the context, use

```cpp
template <typename T>
T* lookupVar(string varName)
T* lookupVar(const Node* varNode)
```
to get the pointer to the variable enumerated by the given node(which should be of type NODE_IDENTIFIER). Use the string version for parallel-for variables, and the int version for all other variables (the reasoning behin this is unfortunate, and is a target for refactoring). If no such variable exists in the current scope, the int version will create and initializ the variable to a default value of 0, false, "", or [] depending on the type. The string version will simply fail an assertion, since parallel for variables should be initialized in another way (See the TetraContext class). Note that you must supply the type of the variable you are referencing. This can usually be obtained by getting the DataType of the node which the interpreter is examining.

```cpp
bool conatinsVar(const std::string varname) const
bool containsVar(const Node*varNode) const
```

These methods state whether or not the scope contains the variable denoted by the string/node is contained within this scope. Similar to lookupVar, the string version should be used for parallel for variables, and the int version for all others.

The TetraScope class uses an enumeration ExecutionStatus to keep track of special conditions the interpreter must account for when executing the program.

####ExecutionStatuses

#####NORMAL
The default status whenever a new scope is created. This simply denotes that the interpreter can continue executing the program as normal.

#####ELIF
This value denotes that the interpreter is currently evaluating nodes of an ELIF statement. As long as the execution flag has this value, the interpreter will continue to evaluate ELIF_CLAUSEs until one of the clauses evaluates to true, or execution reaches the 'else' block. In either case, the interpreter will immediately clear this value.

#####CONTINUE
This value denotes that the interpreter must return program control to the last loop (i.e. when the program uses a 'continue' statement). The interpreter will return from executing its nodes until it returns to the most recent loop node. Then, the condition for the loop node will be reevaluated to determine whether the loop should be executed again.

What this specifically does from the viewpoint of the compiler is that all statement nodes will simply return rather than execute their next child.

#####BREAK
Functions exactly the same as CONTINUE, but for break statemenets. The difference is that when control returns to the most recent loop, the interpreter returns from the loop node rather than checking the condition for the loop. This means that the loop will not be reexecuted even if the condition is true.

#####RETURN
This value denotes that a NODE_RETURN (i.e. 'return' statement) was encountered, and that the interpreter should return the value denoted by the NODE_RETURN node's child. While this flag is set, the interpreter will not execute any children of NODE_STATEMENT nodes, repeat loops or evaluate ELIF conditions. Essentially, the interpreter keeps freturning from evaluating nodes until the current scope is exited via the exitScope() method (see TetraContext).


###TetraContext

The TetraContext class wraps a stack of TetraScopes (explained below), and a TetraScope representing the global scope. A tetra program changes scope whenever it calls a function. It does not change scope when the program executes other blocks, such as if statements and while loops. There should exist exactly one TetraContext per executing thread of the program.

The TetraContext has a default constructor which initializes no data.
Slightly more useful is the constructor:

```cpp
TetraContext(long)
```

which initializes the scope with a given numeric ID (i.e. thread ID)

Once the Tetra Context has been default-constructed, you can call the method

```cpp
initializeGlobalVars(const Node* tree)
```

to initialize the global variables and constants for the program. "tree" should be the base of the abstract syntax tree which the interpreter is executing.

The method
```cpp
void initializeNewScope(Node*)
void initializeNewScope(TetraScope&)
```
pushes a new scope onto the stack initialized with the given information.

When exiting a scope, the method
```cpp
void exitScope()
```
pops the scope off the stack, essentially reverting the program to the state it was at before initializeNewScope() was last called.

When creating a new context for a new thread, the method

```cpp
branchOff(const scope_ptr)
```

will initialize a new scope that is an alias to the scope pointed to by scope_ptr.

The method
```cpp
TetraScope* getCurrentScope()
scope_ptr& getScopeRef()
```
returns a reference to the present scope/scope_ptr of the class.

To lookup a variable, use

```cpp
template <typename T>
T* lookupVar(string varName)
T* lookupVar(const Node* varNode)
```
to get the pointer to the variable referenced by varName, or denoted by the given Node. This will first check the global scope for the variable, and failing finding the variable there, looks in the current scope. The pointer can be dereferenced to get the value of the variable, or to modify the actual contents of the variable.
<!--
In certain cases such as array passing, it is required that two names reference the same element. To facilitate this, you use

```cpp
TData<void*>& declareReference(const string varname)
```
This method returns a blank reference to which varname will match when calling lookupVar(varname). The reference created does not allocate any new memory, and will not flag the scope to delete the memory it is pointed to when it is destroyed. It is up to the caller to specify where the reference should point by using the returned object's setData<void*> function. MAKE SURE TO SPECIFY THE VOID* EXPLICITELY, ELSE THE ASSIGNMENT MAY SILENTLY FAIL.
-->

In the case of parallel-for loops, it is necessary to declare a variable name that means different things depending on which thread is querying the name. To declare such a variable, use

```cpp
void declareThreadSpecificVariable(std::string&)
```

The methods
```cpp
void notifyElif()

void notifyBreak()

void notifyContinue()

void notifyReturn()
```
all set the current scope's execution flag to the specific value. The method

```cpp
void normalizeStatus()
```

clears the execution flag to the normal value.

To find what the current state of the execution flag is, use
```cpp
ExecutionStatus queryExecutionStatus()
```

To manage threads, the class uses the methods:

```cpp
void startParallel()
void addThread(pthread_t)
void endParallel()
```

to initialize a ThreadPool, add threads to the pool, and join with the threads in the ThreadPool. ThreadPools started with startParallel are destroyrd via endParallel on a LIFO basis.

On the note of threads, the method

```cpp
long getThreadID()
```

can be used to get the numerical ID of this context.

When debugging is enabled, the TetraContext will keep track of certain information. This information can be accessed via the following methods:

```cpp
int getLastLineNum()
void* fetchVariable(std::string)
std::map<std::string, int>& getRefTable()
std::map<std::string, int>& getGlobRefTable()
bool isParallelForVariable(std::string)
void registerParallelForVariable(std::string)
void updateVarReferenceTable(const Node*)
void popReferenceTable()
```

getLastLineNum() returns the last line of source code that the context evaluated. fetchVariable() is used to get a pointer to a variable by name. It returns NULL if the variable does not exist. The next two methods are used to get the types of variables by name, so the pointer returned by fetchVariable can be casted to the appropriate type. The isParallelFor function is used to query whether a string identifies a parallel for variable or not. The last three methods are used internally to notify the TetraContext of changes in the program, being the creation of a parallel for variable, the evaluation of a node, and the exiting of a function respectively.


While debugging, the TetraContext also maintains flags and values about its current state. The following are getter and setter methods for those values.

```cpp
bool getStepping()		void setStepping(bool)
bool getStopAtNext()		void setStopAtNext(bool)
bool getResume()		void setResume(bool)
ThreadStatus getRunStatus()	void setRunStatus(ThreadStatus)
```

The three methods represent three flags which a=can be modified and queried by the other parts of the program. Debuggers can use this information to control how the context executes. The last methods set and query an enum of whether the thread is RUNNING, WAITING, or STOPPED.

Lastly, the class provides the method

```cpp
void printStackTrace() 
```

to print the current stack trace.

###scope_ptr
The scope_ptr class is a smart pointer to a TetraScope. Additionally, it manages a collection of threads spawned from the current scope. This class was originally meant to be a private implementation of the TetraContext class, but was made public as a workaround to the problem of threads being able to hold a reference to a scope AND be able to spawn its own threads. This class should later be refactored such that it is reassimilated into the TetraContext, or has a more clearly defined and unique purpose. As a result, some of the interface is unintuitive. Until then, the interface follows:

First, since the operators * an -> can be used to access the underlying TetraScope pointed to by the scope_ptr.

```cpp
scope_ptr(const Node*)
scope_ptr(const TetraScope&)
```

These constructors create a new new scope_ptr pointing to a brand new TetraScope constructed from the information provided.

```cpp
scope_ptr(const scope_ptr&)
```

Unlike the previous constructors, the copy constructor makes a scope_ptr that ALIASES the other TetraScope. However, the new scope_ptr will still have its own collection of threads (i.e. the threads corresponding to each scope are not aliased).

The assignment operator is still under construction. It is not used at present and does not hve the same level of threadsafety as the constructors.

```cpp
void setupParallel()
void endParallel()
```

These methods initialize or destroy a ThreadPool associated with this scope_ptr. This is used to setup and end PARALLEL blocks. Threadpools are created and destroyed on a FIFO basis.

```cpp
void addThread(pthread_t)
```

This method adds a thread to the most current ThreadPool associated with this scope_ptr.

###FunctionMap
The FunctionMap class keeps track of the location of each function. This way, whenever the program calls a function, the interpreter can lookup the function signature and transfer execution to the body of the called function. The FunctionMap is essentially a global map of function names to function bodies.

To create the function table, obtain the base of the program's sytax tree and execute

```cpp
static void build(const Node* tree)
```

This enters all functions in the tree into the function table. Although untested, one could theoretically call this function on multiple different trees so that, as long as each tree does not change its location in memory, a tetra program would be able to call functions from other programs.

To lookup a tetra function, use the function 

```cpp
static const Node* getFunctionNode(const string functionSignature) const
```

to get the NODE_FUNCTION node of the called function. To get the function signature of a called function, use

```cpp
static const Node* getFunctionSugnature(const Node* node) const
```

to extract the function signature from a NODE_FUNCALL node.

###ComparisonList
The ComparisonList class generalizes all comparisons done on all data types. That is, for each type, it implements <, <=, >, >=, ==, !=, and !. Its only method is

```cpp
bool execute(NodeKind kind, TData<T>& op1, TData<T>& op2)
```

This method takes the two pieces of data op1 and op2, performs the operation denoted by the NodeKind kind, and returns the result. Op1 is the left hand side of the operation. If the operation is the unary logical not operator, then op2 is ignored.

This class also defines operations for logical operations not normally seen. Using a logical not on a string datatype compares the string for equality with "". Using the logical not operator on an array type always returns false.

###OperationList
The OperationList class generalizes all mathematical operations done on all data types. In general, if an operation on a type is defined in C++, it is defined here. (i.e. you can  It's only method is

```cpp
T execute(NodeKind kind, TData<T>& op1, TData<T>& op2)
```

This method takes the two pieces of data op1 and op2, performs the operation denoted by the NodeKind kind, and returns the result. Op1 is the left hand side of the operation. If the operation is the unary bitwise not operator, then op2 is ignored.

This class also contains stubbed definitions for operations that make no sense (e.g "abc" ^ "ef"). In the cases where operations are not defined for a specific data type, the operation returns either 0 or "", depending on the primitive type. For array types, these operations return the first argument.

###ThreadPool

	The ThreadPool class defines a group of threads. Its primary use is setting the conditions for the waitTillEmpty() function, which blocks until all the threads in a given thread pool have finished.
	The most used methods follow

```cpp
void addThread(pthread_t)
```

adds the given thread to the threadpool.

```cpp
void waitTillEmpty()
```

blocks until every thread in the pool has finished.

Other methods in ThreadPool include:

```cpp
int queryThreads()
```

returns the number of threads currently in the ThreadPool.

```cpp
void removeThread(pthread_t)
```

removes the specified thread from the ThreadPool. Finally,

```cpp
pthread_t getNextJoin()
```

returns a still-running thread within the ThreadPool.

All the above methods are threadsafe.

###ThreadEnvironment

	The ThreadEnvironment manages two things. One is a ThreadPool of all the threads started in background blocks. To work with the background threads, use the same interface as that presented in the ThreadPool class.
	The second job of this class is to manage all the user-specified mutexes. The sole method dealing with this is

```cpp
pthread_mutex_t* identifyMutex(std::string)
```

which returns the mutex associated with the specified string. If no such mutex exists, a new mutex is created, and a pointer to that new mutex gets returned. This method is threadsafe.

###SystemError & RuntimeError

The SystemError and RuntimeError classes extend Error from the frontend library.

A RuntimeError denotes that the interpreter attempted to perform an illegal operation (e.g. array out of bounds). Its constructor is

```cpp
RuntimeError(std::string message, int lineNumber, TetraContext& context)
```

where context should be the TetraContext where the error occurred.

The given context can be accessed using the method 
```cpp
TetraContext& getContext()
```

A SystemError denotes that the interpreter has entered an invalid state. i.e. it encountered something that made no sense, such as a non-addressable node as the first child of an assignment node, or encountering a datatype that is unrecognized. These can occur if the frontend is more recent than the interpreter. Otherwise, they likely point to a logic error in the interpreter.

The constructor for a SystemError is

```cpp
RuntimeError(std::string message, int lineNumber, Node* node)
```

where node is the node where the SystemError occurred.

The node can be accessed by calling 
```cpp
Node* getNode()
```
###ConsoleArray

The ConsoleArray class is a class used for the interpreter to manage multiple windows of input. As of 4/24/15, it is untested for multiple windows, but the functionality does not hurt single-window execution.

The ConsoleArray class works by maintaining a list of VirtualConsoles. These consoles each get certain threading primitives associated with them according to a user specified policy. That policy is of the form int (policy)(int, bool). Given the threadID and whether or not the message is coming from a debug prompt or not, the user-specified policy should point to the appropriate console number.

To construct a new ConsoleArray, use

```cpp
ConsoleArray()
ConsoleArray(int (...)(int,bool)
```

The latter sets the console policy for the array.

To register a new console, use
```cpp
int registerConsole(VirtualConsole&)
```
The integer returned will be the numeric identifier of the console according to the ConsoleArray. In other words, if the programmer registers console A, and the function returns 2, then the policy function for the console should return 2 in whatever case console A should be used.

To get the a console, use
```cpp
VirtualConsole& getSpecifiedConsole(int, bool) const
```
where int is the thread number, and bool is true if the use of the console is debugging-related.

As mentioned above, the ConsoleArray maintains threading primitives associated with each console. However, it is up to the programmer to properly use these primitives. That is, the ConsoleArray class will not automaticallt limit access to any given console to a single thread, since that is not always the desired bahavior. The ConsoleArray wraps calls to these primitives as follows:

```cpp
void obtainConsoleMutex(int,bool) const
void releaseonsoleMutex(int,bool) const
void waitOnCondition(int,bool) const
void broadcastCondition(int,bool) const
```

These methods invoke the ConsoleArray's Console policy to obtain the appropriate primitive. The calls are anaologous to mutex_lock, mutex_release, condition_wait, and condition_broadcast respectively.

###VirtualConsole

The VirtualConsole class is an abstract class that defines how the interpreter should handle basic I/O (i.e. read and print statements). Environments utilizing this interpreter must create a class extending and overriding the methods of this class, then setting an instance of that class as the observer via the TetraEnvironment's static method setObserver(). For the most basic implementation, the interpreter utilizes an instance of CommandConsole. This is viewable in the interpreter folder.

```cpp
virtual std::string receiveStandardInput() const = 0
```

This method is called whenever the interpreter is expecting input from the user. The method should obtain and return whatever the user input as a string (e.g. return some value obtained via cin). The interpreter takes care of converting it into an appropriate data type.

```cpp
virtual void processStandardOutput(const std::string output) const = 0
```

This method is called whenever the interpreter wishes to print information on the screen. The interpreter passes the information to this method, which should do something to make it visible to the user (e.g. cout the string).

###VirtualObserver

The VirtualObserver class provides a basic framework to debug Tetra programs. As the interpreter is running, it will make calls to the VirtualObserver, which is given great lattitude to do practically whatever it pleases. To utilize the dubugger, use the static setObserver() method in the TetraEnvironment class, then "make" the interpreter with the DEBUG target. The current basic implementation is the CommandObserver, which can be viewed in the interpreter folder. The observer presently exhibits default behavior for multithreaded programs, and depends partially on the Nodes haviing the correct associated line numbers.

```cpp
virtual void notify_E(const Node* node, TetraContext& context) = 0
```

The interpreter calls this function immediately before it executes node. It passes the current context as well. note that the context is not const.

```cpp
virtual void leftScope_E() = 0
```

The interpreter calls this method immediately after exiting a scope (i.e. returning from a function), but before calling notify_E for the next node. Some implementations of a debugger may not need this functionality, in which case the method can be stubbed.

#####A Word About the testProgs Directory
The interpreter folder contains a series of tetra programs, and a shell script that executes each program and tests its return value against a list of expected return values. Going to this directory and executing "sh runTests.sh" will execute all the test programs, and give information about the successfailure of each program. This can be used as a quick check to insure that changes to the interpreter did not break previous functionality. To add additional tests, create a file called [*]Test.ttr, copy and paste one of the lines in the shell script defining the test cases, and fill in the values of [*] for the name and the expected return value.
