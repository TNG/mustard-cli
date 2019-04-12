# Poor man's DI

## Compiling and testing
    mkdir build
    cmake ..
    make -j20
    ../bin/poorMansDI-tests

## Configuring PMDI in your project
PMDI needs to be created before all provided classes. Thus you need to make sure that it is linked first.
Otherwise you need to make sure that no object is being provided before the main method is called.

Thus, the best practise is to include the src/ folder of PMDI into your project.

## Basic usage
### Example
#### providing components
When you want to mark a class `Apple` to be provided as a dependency, simply create an object

    ProvideDependency<Apple> appleDependency;
    
For the implementation of an interface, you write

    ProvideImplementationFor<Apple, AppleIface> appleImplDependency;

Only while these depency-objects exist you will be able to get a reference to a singleton of the `Apple` class.
The Singleton is created when it is first asked for. You can provide components by writing static dependencies
into your `.cpp` files, or e.g. by creating a configuration class.

Note that dependencies can be overwritten and the latest created provider will determine which class will
be created when asking for a certain interface. When the latest provider goes out of 
scope, the previous provider is in place again.

All references to objects provided, however, will stay valid
until the application quits.
    
#### depending on components
When you rely on a Component to be present, you can get a pointer to a singleton by

    Apple *appleSingleton = DependentOn<Apple>();

or for an interface
    
    AppleIface *appleIface = DependentOnA<AppleIface>();
    
The best practise to insert dependencies into classes is to do this via a constructor that has a
`nullptr` as the default argument. When the argument is not null, the pointer that has been provided
by the user will be used.

    class FruitSalad{
      public:
        FruitSalad( Apple *apple = nullptr):
            apple(DependentOn<Apple>(apple))
        {}
        ...
      private:
      Apple *apple;
    }
While this is very suggestive, be aware that in the constructor, `apple` references the (possibly `==null`)
argument to the constructor, while `this->apple` references the instance. PMDI will provide an instance iff
the argument has been `nullptr` and there has been provided a class to create using `Provide...Dependency` 
objects.

### 'Techday' Demo
You'll find a working demonstration of PMDI in action in the example folder. 
It shows you the basics of providing and depending on components.

