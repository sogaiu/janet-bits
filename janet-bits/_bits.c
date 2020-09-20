#include <janet.h>

static Janet cfun_bits_to_buffer(int32_t argc, Janet* argv) {
  janet_fixarity(argc, 1);
  JanetBuffer* buffer;
  if (janet_checkint(argv[0])) {
    const uint32_t x = janet_getinteger(argv, 0);
    buffer = janet_buffer(sizeof(uint32_t) * 8);
    uint32_t b;
    uint8_t c;
    for (uint32_t i = 0;
         i < sizeof(uint32_t); i++) {    // loop over the four bytes
      b = (x >> (8 * i)) & 0xFF;         // select the ith byte
      for (uint32_t j = 0; j < 8; j++) { // loop over the bits of the ith byte
        if (b & (1 << j)) {              // is the current bit a 1?
          c = '1';
        } else {
          c = '0';
        }
        buffer->data[(8 * i) + j] = c;
      }
    }
    buffer->count += sizeof(uint32_t) * 8;
  } else if (janet_checktype(argv[0], JANET_NUMBER)) {
    const double d = janet_getnumber(argv, 0);
    char* ptr = (char*)&d;
    buffer = janet_buffer(sizeof(double) * 8);
    char b;
    uint8_t c;
    for (uint32_t i = 0;
         i < sizeof(double); i++) {      // loop over the eight bytes
      b = *ptr;                          // select the next byte
      ptr++;
      for (uint32_t j = 0; j < 8; j++) { // loop over the bits of the ith byte
        if (b & (1 << j)) {              // is the current bit a 1?
          c = '1';
        } else {
          c = '0';
        }
        buffer->data[(8 * i) + j] = c;
      }
    }    
    buffer->count += sizeof(double) * 8;
  } else if (janet_checkabstract(argv[0], &janet_u64_type)) {
    const uint64_t x =
      *((uint64_t*)janet_getabstract(argv, 0, &janet_u64_type));
    buffer = janet_buffer(sizeof(uint64_t) * 8);
    uint32_t b;
    uint8_t c;
    for (uint32_t i = 0;
         i < sizeof(uint64_t); i++) {    // loop over the eight bytes
      b = (x >> (8 * i)) & 0xFF;         // select the ith byte
      for (uint32_t j = 0; j < 8; j++) { // loop over the bits of the ith byte
        if (b & (1 << j)) {              // is the current bit a 1?
          c = '1';
        } else {
          c = '0';
        }
        buffer->data[(8 * i) + j] = c;
      }
    }
    buffer->count += sizeof(uint64_t) * 8;
  } else {
    return janet_wrap_nil();
  }
  return janet_wrap_buffer(buffer);
}

static Janet cfun_bits_as_uint32_to_buffer(int32_t argc, Janet* argv) {
  janet_fixarity(argc, 1);
  JanetBuffer* buffer;
  if (janet_checkint(argv[0])) {
    const uint32_t x = janet_getinteger(argv, 0);
    buffer = janet_buffer(sizeof(uint32_t) * 8);
    uint32_t b;
    uint8_t c;
    for (uint32_t i = 0;
         i < sizeof(uint32_t); i++) {    // loop over the four bytes
      b = (x >> (8 * i)) & 0xFF;         // select the ith byte
      for (uint32_t j = 0; j < 8; j++) { // loop over the bits of the ith byte
        if (b & (1 << j)) {              // is the current bit a 1?
          c = '1';
        } else {
          c = '0';
        }
        buffer->data[(8 * i) + j] = c;
      }
    }
    buffer->count += sizeof(uint32_t) * 8;
  } else {
    return janet_wrap_nil();
  }
  return janet_wrap_buffer(buffer);
}

static Janet cfun_bits_as_double_to_buffer(int32_t argc, Janet* argv) {
  janet_fixarity(argc, 1);
  JanetBuffer* buffer;
  if (janet_checktype(argv[0], JANET_NUMBER)) {
    const double d = janet_getnumber(argv, 0);
    char* ptr = (char*)&d;
    buffer = janet_buffer(sizeof(double) * 8);
    char b;
    uint8_t c;
    for (uint32_t i = 0;
         i < sizeof(double); i++) {      // loop over the eight bytes
      b = *ptr;                          // select the next byte
      ptr++;
      for (uint32_t j = 0; j < 8; j++) { // loop over the bits of the ith byte
        if (b & (1 << j)) {              // is the current bit a 1?
          c = '1';
        } else {
          c = '0';
        }
        buffer->data[(8 * i) + j] = c;
      }
    }    
    buffer->count += sizeof(double) * 8;
  } else {
    return janet_wrap_nil();
  }
  return janet_wrap_buffer(buffer);
}

static Janet cfun_bits_as_uint64_to_buffer(int32_t argc, Janet* argv) {
  janet_fixarity(argc, 1);
  JanetBuffer* buffer;
  if (janet_checkabstract(argv[0], &janet_u64_type)) {
    const uint64_t x =
      *((uint64_t*)janet_getabstract(argv, 0, &janet_u64_type));
    buffer = janet_buffer(sizeof(uint64_t) * 8);
    uint32_t b;
    uint8_t c;
    for (uint32_t i = 0;
         i < sizeof(uint64_t); i++) {    // loop over the eight bytes
      b = (x >> (8 * i)) & 0xFF;         // select the ith byte
      for (uint32_t j = 0; j < 8; j++) { // loop over the bits of the ith byte
        if (b & (1 << j)) {              // is the current bit a 1?
          c = '1';
        } else {
          c = '0';
        }
        buffer->data[(8 * i) + j] = c;
      }
    }
    buffer->count += sizeof(uint64_t) * 8;
  } else {
    return janet_wrap_nil();
  }
  return janet_wrap_buffer(buffer);
}

static const JanetReg cfuns[] = {
  {"to-buffer", cfun_bits_to_buffer,
   "(_bits/to-buffer value)\n\n"
   "Return the bit pattern of value as a buffer."},
  {"uint32-bits", cfun_bits_as_uint32_to_buffer,
   "(_bits/uint32-bits value)\n\n"
   "Return the bit pattern of value as a uint32 in a buffer."},
  {"double-bits", cfun_bits_as_double_to_buffer,
   "(_bits/double-bits value)\n\n"
   "Return the bit pattern of value as a double in a buffer."},
  {"uint64-bits", cfun_bits_as_uint64_to_buffer,
   "(_bits/uint64-bits value)\n\n"
   "Return the bit pattern of value as a uint64 in a buffer."},
  {NULL, NULL, NULL}
};

JANET_MODULE_ENTRY(JanetTable* env) {
  janet_cfuns(env, "_bits", cfuns);
}
