#include <iostream>
#include <vector>
#include <cstring>
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"

int main() {
    std::cout << "=============================================" << std::endl;
    std::cout << "  SAM9X60 Battery System Inference Runtime   " << std::endl;
    std::cout << "=============================================" << std::endl;

    while (true) {
        float input_voltage = 0.0f;
        float input_current = 0.0f;
        float input_temperature = 0.0f;

        // 1. Interactive Runtime Prompts
        std::cout << "\nEnter sensor readings (or enter '-1' to exit):" << std::endl;
        
        std::cout << "Voltage (V): ";
        if (!(std::cin >> input_voltage) || input_voltage == -1.0f) break;

        std::cout << "Current (A): ";
        if (!(std::cin >> input_current)) break;

        std::cout << "Temperature (C): ";
        if (!(std::cin >> input_temperature)) break;

        // 2. Pack the variables into the features array
        float features[3] = { input_voltage, input_current, input_temperature };

        // 3. Package the array into the Edge Impulse signal wrapper
        signal_t signal;
        signal.total_length = 3;
        signal.get_data = [&features](size_t offset, size_t length, float *out_ptr) -> int {
            std::memcpy(out_ptr, features + offset, length * sizeof(float));
            return 0;
        };

        // 4. Run the Edge Impulse classifier loop
        ei_impulse_result_t result = { 0 };
        EI_IMPULSE_ERROR err = run_classifier(&signal, &result, false);
        
        if (err != EI_IMPULSE_OK) {
            std::cout << "Error: Model classification failed (" << err << ")" << std::endl;
            continue;
        }

        // 5. Print results output breakdown
        std::cout << "\n>>> PREDICTION RESULTS <<<" << std::endl;
        for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
            std::cout << "  State: " << result.classification[ix].label 
                      << " | Confidence: " << (result.classification[ix].value * 100.0f) << "%" 
                      << std::endl;
        }
        std::cout << "---------------------------------------------" << std::endl;
    }

    std::cout << "\nExiting application. Goodbye!" << std::endl;
    return 0;
}
