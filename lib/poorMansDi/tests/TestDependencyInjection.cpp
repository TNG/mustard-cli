#include <gtest/gtest.h>
#include <Provide.h>
#include <Depend.h>
#include <Common.h>

using namespace testing;


class SimpleClass
{
public:
    void setTest ( double x ) {
        test = x;
    };

    double getTest() const {
        return test;
    }

private:
    double test;
};

class ComplexClass
{
public:

    //Provide two constructors, one with dependencies injected, one for testing purposes.
    ComplexClass() : simpleClass ( DependentOn<SimpleClass>() ) { }
    ComplexClass ( SimpleClass &simpleClass ) : simpleClass ( simpleClass ) {}

    SimpleClass *getSimpleClass() {
        return &simpleClass;
    }

private:
    SimpleClass &simpleClass;
};


//Put the ProvideDependency instance into the .cpp class file, make sure to link this class file.
ProvideDependency<SimpleClass> simpleClassDescription;
ProvideDependency<ComplexClass> complexClassDescription;

TEST ( DependencyInjection, Unit_simpleDependency )
{
    SimpleClass *simpleClass = DependentOn<SimpleClass>();
    simpleClass->setTest ( 10. );
    ASSERT_TRUE ( simpleClass != nullptr );
    SimpleClass *simpleClass2 = DependentOn<SimpleClass>();
    ASSERT_TRUE ( simpleClass == simpleClass2 );
}

TEST ( DependencyInjection, Unit_complexClass )
{
    ComplexClass complexSimplyCreated;
    ComplexClass *complexComplexCreated = DependentOn<ComplexClass>();
    complexSimplyCreated.getSimpleClass()->getTest();
    ASSERT_TRUE ( nullptr != complexComplexCreated );
    ASSERT_TRUE ( complexComplexCreated->getSimpleClass() == complexSimplyCreated.getSimpleClass() );
}


class TestInterface
{
public:
    virtual int getShittyAnswer() = 0;
};

class SolidImplementation: public TestInterface
{
public:
    virtual int getShittyAnswer() override {
        return 42;
    }
    virtual ~SolidImplementation() {}
};

class OtherSolidImplementation: public TestInterface
{
public:
    virtual int getShittyAnswer() override {
        return 1337;
    }
    virtual ~OtherSolidImplementation() {}
};

ProvideImplementationForDependency<SolidImplementation, TestInterface> implementationBinder;


TEST ( DependencyInjection, Unit_provideInterfaceImplementation )
{
    TestInterface *someTestInterface = DependentOnA<TestInterface>();
    ASSERT_TRUE ( nullptr != someTestInterface );
    ASSERT_EQ ( 42, someTestInterface->getShittyAnswer() );
}

class ThatNeedsTestInterface
{
public:
    ThatNeedsTestInterface ( TestInterface *testInterface = nullptr )
        : testInterface ( DependentOnA<TestInterface> ( testInterface ) ) {};

    int getShittyAnswer() {
        return testInterface->getShittyAnswer();
    }
private:
    TestInterface *testInterface;
};

TEST ( DependencyInjection, Unit_overrideDependency )
{
    OtherSolidImplementation eliteImplementation;

    ThatNeedsTestInterface classWithProvidedDependency;
    ThatNeedsTestInterface classWithGivenDependency ( &eliteImplementation );

    ASSERT_EQ ( 42, classWithProvidedDependency.getShittyAnswer() );
    ASSERT_EQ ( 1337, classWithGivenDependency.getShittyAnswer() );
}

TEST ( DependencyContainer, Unit_overrideDependency )
{
    TestInterface *testInterfaceDefault = DependentOn<TestInterface>();
    EXPECT_EQ ( 42, testInterfaceDefault->getShittyAnswer() );

    {
        ProvideImplementationForDependency<OtherSolidImplementation, TestInterface> overrideDependency;
        TestInterface *overriddenInterface = DependentOn<TestInterface>();
        EXPECT_EQ ( 1337, overriddenInterface->getShittyAnswer() );
    }
    TestInterface *restoredDefault = DependentOn<TestInterface>();
    EXPECT_EQ ( testInterfaceDefault, restoredDefault );
    EXPECT_EQ ( 42, restoredDefault->getShittyAnswer() );
}

TEST ( DependencyContainer, Unit_overrideUninitializedDependency )
{
    {
        ProvideImplementationForDependency<OtherSolidImplementation, TestInterface> overrideDependency;
        TestInterface *overriddenInterface = DependentOn<TestInterface>();
        EXPECT_EQ ( 1337, overriddenInterface->getShittyAnswer() );
    }
    TestInterface *defaultDependency = DependentOn<TestInterface>();
    EXPECT_EQ ( 42, defaultDependency->getShittyAnswer() );
}

TEST ( DependencyContainer, Unit_testSingletonWillLiveAfterDependency )
{
    TestInterface *overriddenInterface = nullptr;
    {
        ProvideImplementationForDependency<OtherSolidImplementation, TestInterface> overrideDependency;
        overriddenInterface = DependentOn<TestInterface>();
        EXPECT_EQ ( 1337, overriddenInterface->getShittyAnswer() );
    }
    TestInterface *defaultDependency = DependentOn<TestInterface>();
    EXPECT_EQ ( 42, defaultDependency->getShittyAnswer() );
    EXPECT_EQ ( 1337, overriddenInterface->getShittyAnswer() );
}

class InnerClass
{
public:
    void touch() {
        isTouched = true;
    }
    ~InnerClass() {
        if ( !isTouched ) {
            errmsg ( "Inner class was not touched!" );
            assert ( isTouched );
        }
    }
private:
    bool isTouched = false;
};

class OuterClass
{
public:
    OuterClass() : innerClass ( DependentOn<InnerClass>() ) {};
    ~OuterClass() {
        innerClass->touch();
    }
private:
    InnerClass *innerClass;
};

TEST ( DependencyContainer, Unit_InnerClassDestroyedAfterOuterClass )
{
    //This test will only fail at exit if something went wrong with class destruction sequence.
    ProvideDependency<InnerClass> innerClassDependency;
    ProvideDependency<OuterClass> outerClassDependency;
    __attribute__ ( ( unused ) ) OuterClass *outerClass = DependentOn<OuterClass>();
}

class BaseA
{
public:
    virtual int returnAnInt() = 0;
    virtual ~BaseA() {}
};

class BaseB
{
public:
    virtual double returnADouble() = 0;
    virtual ~BaseB() {}
};
class DerivedA : public BaseA
{
public:
    virtual int returnAnInt() override {
        return 1337;
    }
    virtual ~DerivedA() {}
};
class DerivedB: public BaseB
{
public:
    virtual double returnADouble() override {
        return 42.;
    }
};
class AFirstBSecond : public DerivedA, public DerivedB
{

};

class BFirstASecond : public DerivedB, public DerivedA
{

};

TEST ( DependencyInjection, Unit_HandleMultipleInheritanceCorrectly )
{
    {
        ProvideImplementationForDependency<AFirstBSecond, DerivedA> derivedADependency;
        ProvideImplementationForDependency<AFirstBSecond, DerivedB> derivedBDependency;
        DerivedA *derivedA = DependentOnA<DerivedA>();
        DerivedB *derivedB = DependentOnA<DerivedB>();

        EXPECT_EQ ( derivedA->returnAnInt(), 1337 );
        EXPECT_EQ ( derivedB->returnADouble(), 42 );
    }
    {
        ProvideImplementationForDependency<BFirstASecond, DerivedA> derivedADependency;
        ProvideImplementationForDependency<BFirstASecond, DerivedB> derivedBDependency;
        DerivedA *derivedA = DependentOnA<DerivedA>();
        DerivedB *derivedB = DependentOnA<DerivedB>();

        EXPECT_EQ ( derivedA->returnAnInt(), 1337 );
        EXPECT_EQ ( derivedB->returnADouble(), 42 );
    }
}

