file (GLOB EXAMPLE_SOURCE_FILES
        "../src/*.cpp"
        "main.cpp"
        "CoffeeMachines/Barista.cpp"
        "CoffeeMachines/KlixMachine.cpp"
        "Techday.cpp"
        "TechdayConfiguration.cpp"
        )
add_executable(poorMansDi-TechdayDemo ${EXAMPLE_SOURCE_FILES})