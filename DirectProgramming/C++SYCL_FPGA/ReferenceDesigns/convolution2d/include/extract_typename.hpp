//  Copyright (c) 2024 Intel Corporation
//  SPDX-License-Identifier: MIT

// extract_typename.hpp

#pragma once

// C++ magic that lets us extract template parameters from SYCL pipes,
// `StreamingBeat` structs based off code from https://ideone.com/SDEgq

#include <stdint.h>

#include <sycl/ext/intel/prototype/pipes_ext.hpp>

template <typename T>
struct ExtractValueType {
  typedef T value_type;
};

template <template <typename, typename...> typename X, typename T,
          typename... Args>
struct ExtractValueType<X<T, Args...>>  // specialization
{
  typedef T value_type;
};

template <typename T>
struct ExtractPipeType {
  typedef T value_type;
};

template <template <class, class, int32_t, class, typename...> class PIPE_CLASS,
          class PIPE_NAME, class PIPE_DATA_T, int32_t PIPE_MIN_CAPACITY,
          class PIPE_PROPERTIES, typename... Args>
struct ExtractPipeType<
    PIPE_CLASS<PIPE_NAME, PIPE_DATA_T, PIPE_MIN_CAPACITY, PIPE_PROPERTIES,
               Args...>>  // specialization
{
  typedef PIPE_DATA_T value_type;
};

template <typename T>
struct ExtractStreamingBeatType {
  typedef T value_type;
  static constexpr bool use_packets = false;
  static constexpr bool use_empty = false;
};

template <template <class, bool, bool> class BEAT_CLASS, class BEAT_DATA_T,
          bool BEAT_USE_PACKETS, bool BEAT_EMPTY>
struct ExtractStreamingBeatType<
    BEAT_CLASS<BEAT_DATA_T, BEAT_USE_PACKETS, BEAT_EMPTY>>  // specialization
{
  typedef BEAT_DATA_T value_type;
  static constexpr bool use_packets = BEAT_USE_PACKETS;
  static constexpr bool use_empty = BEAT_EMPTY;
};

template <typename PipeWithStreamingType>
using BeatPayload = typename ExtractStreamingBeatType<
    typename ExtractPipeType<PipeWithStreamingType>::value_type>::value_type;

template <typename PipeWithStreamingType>
constexpr bool BeatUsePackets() {
  return ExtractStreamingBeatType<typename ExtractPipeType<
      PipeWithStreamingType>::value_type>::use_packets;
}

template <typename PipeWithStreamingType>
constexpr bool BeatUseEmpty() {
  return ExtractStreamingBeatType<
      typename ExtractPipeType<PipeWithStreamingType>::value_type>::use_empty;
}