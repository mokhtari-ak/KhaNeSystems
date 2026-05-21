#pragma once

/**
 * @brief Configuration stricte pour Eigen afin de garantir 
 * le respect du contrat zéro-allocation dans l'embarqué.
 */

// Interdire toute allocation dynamique dans Eigen
#define EIGEN_NO_MALLOC

// Désactiver les exceptions C++ (obligatoire pour notre projet)
#define EIGEN_NO_EXCEPTIONS

// Désactiver le multithreading OpenMP (inutile sur STM32)
#define EIGEN_DONT_PARALLELIZE

// Optimisation pour processeurs ARM (si supporté par le toolchain)
// #define EIGEN_VECTORIZE_NEON 

#include <Eigen/Dense>

namespace FlightMath {
    // Types standards pour notre EKF
    // Quaternion (4) + Gyro Bias (3) = 7 états
    using StateVector = Eigen::Matrix<float, 7, 1>;
    using StateMatrix = Eigen::Matrix<float, 7, 7>;
    
    // Mesure Accel (3) + Gyro (3) = 6 mesures
    using MeasurementVector = Eigen::Matrix<float, 6, 1>;
    using MeasurementMatrix = Eigen::Matrix<float, 6, 6>;
}
