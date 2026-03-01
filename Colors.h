#ifndef COLORS_H
#define COLORS_H

/**
 * @file Colors.h
 * @brief ANSI color escape codes used to highlight console output.
 */

/** @brief Reset all attributes. */
#define COLOR_RESET  "\033[0m"

/** @brief Green text for per-server messages. */
#define COLOR_GREEN  "\033[32m"

/** @brief Yellow text for scaling events (scale up / scale down). */
#define COLOR_YELLOW "\033[33m"

/** @brief Red text for periodic tick / status lines. */
#define COLOR_RED    "\033[31m"

#endif
