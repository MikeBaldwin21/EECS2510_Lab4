/*
 * Constants.h
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 11/24/2019
 * Description: Declaration of constants used in the program
*/

#pragma once

constexpr int T = 99; // <=============== DR. THOMAS, ONLY CHANGE THIS!!! ===============\\ <=== Pump these rookie numbers up

constexpr int MIN_KEY = T - 1; // Minimum amount of keys allowed per node
constexpr int MAX_KEY = (2 * T - 1); // Maximum amount of keys allowed per node
constexpr int MIN_CHILDREN = T; // Minimum amount of children allowed per node
constexpr int MAX_CHILDREN = (2 * T); // Maximum amount of children allowed per node
constexpr int MAX_DATA_LENGTH = 50; // Largest word allocated (based upon PDF info)
//static_assert(T % 2 != 0, "T must be an odd number!"); // According to Lecture 16 - slide #16 (Last sentence)
static_assert(T >= 2, "T must be greater than 2!");
static_assert(MIN_KEY <= MAX_KEY, "MIN_KEY must be less than or equal to MAX_KEY!");
static_assert(T >= 2, "T must be greater than 2!");
static_assert(MIN_CHILDREN <= MAX_CHILDREN, "MIN_CHILDREN must be less than or equal to MAX_DATA_LENGTH!");
