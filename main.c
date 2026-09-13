#include <stdio.h>

#include "raylib.h"
#include "grid.h"
#include "wire.h"
#include "component.h"
#include "circuit.h"
#include "netlist.h"
#include "solver.h"


typedef enum
{
    TOOL_WIRE,
    TOOL_RESISTOR,
    TOOL_DC_SOURCE,
    TOOL_SELECT,
    TOOL_GROUND
} ToolMode;


ToolMode SelectMode(ToolMode currentTool, bool* wireStarted)
{
    if (IsKeyPressed(KEY_W))
    {
        return TOOL_WIRE;
    }

    if (IsKeyPressed(KEY_R))
    {
        *wireStarted = false;
        return TOOL_RESISTOR;
    }

    if (IsKeyPressed(KEY_D))
    {
        *wireStarted = false;
        return TOOL_DC_SOURCE;
    }

    if (IsKeyPressed(KEY_E))
    {
        *wireStarted = false;
        return TOOL_SELECT;
    }

    if (IsKeyPressed(KEY_G))
    {
        *wireStarted = false;
        return TOOL_GROUND;
    }

    return currentTool;
}


const char* GetModeName(ToolMode mode)
{
    switch (mode)
    {
    case TOOL_WIRE:
        return "WIRE";

    case TOOL_RESISTOR:
        return "RESISTOR";

    case TOOL_DC_SOURCE:
        return "DC SOURCE";

    case TOOL_SELECT:
        return "EDIT";

    case TOOL_GROUND:
        return "GROUND";

    default:
        return "UNKNOWN";
    }
}


int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 700;

    InitWindow(screenWidth, screenHeight, "VoltCraft");
    SetTargetFPS(60);


    // Wire data
    Wire wire[100];
    int wireCount = 0;

    bool wireStarted = false;
    Vector2 wireStart = { 0, 0 };


    // Resistors
    Resistor resistor[100];
    int resistorCount = 0;


    // DC sources
    DCSource dcSource[100];
    int dcSourceCount = 0;


    // Ground
    bool groundPlaced = false;
    Vector2 groundPosition = { 0, 0 };


    // Selection
    int selectedResistor = -1;
    int selectedDCSource = -1;


    // Circuit / BFS data
    CircuitData circuit;
    int nodeNumber[MAX_POINTS];


    ToolMode currentTool = TOOL_WIRE;


    while (!WindowShouldClose())
    {
        Vector2 mousePosition = GetMousePosition();
        Vector2 snappedPosition = SnapToGrid(mousePosition);

        currentTool = SelectMode(
            currentTool,
            &wireStarted
        );


        // -------------------------------------------------
        // Clear circuit
        // -------------------------------------------------

        if (IsKeyPressed(KEY_DELETE))
        {
            wireCount = 0;
            resistorCount = 0;
            dcSourceCount = 0;

            wireStarted = false;

            selectedResistor = -1;
            selectedDCSource = -1;

            groundPlaced = false;
        }


        // -------------------------------------------------
        // Solve circuit
        // -------------------------------------------------

        if (IsKeyPressed(KEY_S))
        {
            bool validCircuit = true;

            for (int i = 0; i < resistorCount; i++)
            {
                if (resistor[i].value == 0)
                {
                    validCircuit = false;
                }
            }

            for (int i = 0; i < dcSourceCount; i++)
            {
                if (dcSource[i].value == 0)
                {
                    validCircuit = false;
                }
            }


            if (!groundPlaced)
            {
                printf(
                    "\nERROR: Ground has not been placed.\n"
                );
            }

            else if (!validCircuit)
            {
                printf(
                    "\nERROR: Give every component a non-zero value.\n"
                );
            }

            else if (BuildCircuitData(
                wire,
                wireCount,
                resistor,
                resistorCount,
                dcSource,
                dcSourceCount,
                groundPlaced,
                groundPosition,
                &circuit))
            {
                int totalNodes =
                    assignElectricalNodes(
                        circuit.pointCount,
                        circuit.groundPoint,
                        circuit.graph,
                        nodeNumber
                    );


                printPointMapping(
                    circuit.pointCount,
                    nodeNumber
                );


                printNetlist(
                    circuit.components,
                    circuit.componentCount,
                    nodeNumber
                );


                int activeNodes =
                    totalNodes - 1;


                buildAndSolveCircuit(
                    circuit.components,
                    circuit.componentCount,
                    activeNodes,
                    nodeNumber
                );
            }

            else
            {
                printf(
                    "\nERROR: Circuit data could not be generated.\n"
                );
            }
        }


        // -------------------------------------------------
        // Wire tool
        // -------------------------------------------------

        if (currentTool == TOOL_WIRE)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                wireStarted = false;
            }


            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (!wireStarted)
                {
                    wireStart = snappedPosition;
                    wireStarted = true;
                }

                else
                {
                    if (wireCount < 100)
                    {
                        wire[wireCount].start = wireStart;
                        wire[wireCount].end = snappedPosition;

                        wireCount++;

                        wireStarted = false;
                    }
                }
            }
        }


        // -------------------------------------------------
        // Resistor placement
        // -------------------------------------------------

        if (currentTool == TOOL_RESISTOR)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (resistorCount < 100)
                {
                    resistor[resistorCount].position =
                        snappedPosition;

                    resistor[resistorCount].value = 0;

                    resistorCount++;
                }
            }
        }


        // -------------------------------------------------
        // DC source placement
        // -------------------------------------------------

        if (currentTool == TOOL_DC_SOURCE)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (dcSourceCount < 100)
                {
                    dcSource[dcSourceCount].position =
                        snappedPosition;

                    dcSource[dcSourceCount].value = 0;

                    dcSourceCount++;
                }
            }
        }


        // -------------------------------------------------
        // Ground placement
        // -------------------------------------------------

        if (currentTool == TOOL_GROUND)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                groundPosition = snappedPosition;
                groundPlaced = true;
            }
        }


        // -------------------------------------------------
        // Component selection / editing
        // -------------------------------------------------

        if (currentTool == TOOL_SELECT)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                selectedResistor = -1;
                selectedDCSource = -1;


                for (int i = 0; i < resistorCount; i++)
                {
                    Rectangle resistorBox =
                    {
                        resistor[i].position.x - 50,
                        resistor[i].position.y - 15,
                        100,
                        30
                    };

                    if (CheckCollisionPointRec(
                        mousePosition,
                        resistorBox))
                    {
                        selectedResistor = i;
                        break;
                    }
                }


                for (int i = 0; i < dcSourceCount; i++)
                {
                    Rectangle sourceBox =
                    {
                        dcSource[i].position.x - 30,
                        dcSource[i].position.y - 50,
                        60,
                        100
                    };

                    if (CheckCollisionPointRec(
                        mousePosition,
                        sourceBox))
                    {
                        selectedDCSource = i;
                        selectedResistor = -1;
                        break;
                    }
                }
            }


            int key = GetCharPressed();

            if (key >= '0' && key <= '9')
            {
                int digit = key - '0';

                if (selectedResistor != -1)
                {
                    resistor[selectedResistor].value =
                        resistor[selectedResistor].value * 10
                        + digit;
                }

                else if (selectedDCSource != -1)
                {
                    dcSource[selectedDCSource].value =
                        dcSource[selectedDCSource].value * 10
                        + digit;
                }
            }
        }


        // =================================================
        // DRAWING
        // =================================================

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawCircuitGrid();


        // Wires
        for (int i = 0; i < wireCount; i++)
        {
            DrawWire(
                wire[i].start,
                wire[i].end
            );
        }


        // Resistors
        for (int i = 0; i < resistorCount; i++)
        {
            DrawResistor(
                resistor[i].position
            );

            char valueText[20];

            sprintf_s(
                valueText,
                sizeof(valueText),
                "%.0f ohm",
                resistor[i].value
            );

            DrawText(
                valueText,
                resistor[i].position.x - 25,
                resistor[i].position.y - 35,
                18,
                BLACK
            );
        }


        // DC sources
        for (int i = 0; i < dcSourceCount; i++)
        {
            DrawDCSource(
                dcSource[i].position
            );

            char valueText[20];

            sprintf_s(
                valueText,
                sizeof(valueText),
                "%.0f V",
                dcSource[i].value
            );

            DrawText(
                valueText,
                dcSource[i].position.x + 35,
                dcSource[i].position.y - 10,
                18,
                BLACK
            );
        }


        // Selected resistor
        if (selectedResistor != -1)
        {
            Rectangle resistorBox =
            {
                resistor[selectedResistor].position.x - 50,
                resistor[selectedResistor].position.y - 15,
                100,
                30
            };

            DrawRectangleLinesEx(
                resistorBox,
                2,
                BLUE
            );
        }


        // Selected source
        if (selectedDCSource != -1)
        {
            Rectangle sourceBox =
            {
                dcSource[selectedDCSource].position.x - 30,
                dcSource[selectedDCSource].position.y - 50,
                60,
                100
            };

            DrawRectangleLinesEx(
                sourceBox,
                2,
                BLUE
            );
        }


        // Ground
        if (groundPlaced)
        {
            DrawCircleV(
                groundPosition,
                6,
                BLUE
            );
        }


        // Wire preview
        if (currentTool == TOOL_WIRE)
        {
            DrawCircleV(
                snappedPosition,
                4,
                RED
            );

            if (wireStarted)
            {
                DrawWire(
                    wireStart,
                    snappedPosition
                );
            }
        }


        // Resistor preview
        if (currentTool == TOOL_RESISTOR)
        {
            DrawResistor(
                snappedPosition
            );
        }


        // Source preview
        if (currentTool == TOOL_DC_SOURCE)
        {
            DrawDCSource(
                snappedPosition
            );
        }


        // Ground preview
        if (currentTool == TOOL_GROUND)
        {
            DrawCircleV(
                snappedPosition,
                4,
                RED
            );
        }


        // Mode display
        DrawText(
            TextFormat(
                "Mode: %s",
                GetModeName(currentTool)
            ),
            20,
            20,
            20,
            DARKGRAY
        );


        DrawText(
            "W: Wire | R: Resistor | D: DC | E: Edit | G: Ground | S: Solve | Delete: Clear",
            20,
            45,
            16,
            DARKGRAY
        );


        EndDrawing();
    }


    CloseWindow();

    return 0;
}