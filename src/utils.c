// © 2014 Abimael Martell
// System Monitor - multi-platform system monitor
// See LICENSE

#include "system_monitor.h"

static int sigar_inet_ntoa (sigar_uint32_t address, char *addr_str)
{
    char *next = addr_str;
    int n = 0;
    const unsigned char *src = (const unsigned char *)&address;

    do {
        unsigned char u = *src++;
        if (u > 99) {
            *next++ = '0' + u/100;
            u %= 100;
            *next++ = '0' + u/10;
            u %= 10;
        }
        else if (u > 9) {
            *next++ = '0' + u/10;
            u %= 10;
        }
        *next++ = '0' + u;
        *next++ = '.';
        n++;
    } while (n < 4);

    *--next = 0;

    return SIGAR_OK;
}

char *inet_to_string (sigar_uint32_t inet)
{
    char address[SIGAR_INET6_ADDRSTRLEN], *address_ptr;
    int rc;

    rc = sigar_inet_ntoa(inet, address);

    if (rc == SIGAR_OK) {
        address_ptr = strdup(address);
        return address_ptr;
    }

    return NULL;
}

char *mac_address_to_string (unsigned char mac[8])
{
    char mac_string[MAC_ADDRESS_STRING_LENGTH], *mac_ptr;

    snprintf(
        mac_string,
        sizeof(mac_string),
        MAC_ADDRESS_FORMAT,
        (mac[0] & 0xff), (mac[1] & 0xff), (mac[2] & 0xff),
        (mac[3] & 0xff), (mac[4] & 0xff), (mac[5] & 0xff)
    );

    mac_ptr = strdup(mac_string);
  
    return mac_ptr;
}

/* from vendor/mongoose/mongoose.c mg_url_encode, though not the same
 * exactly */
void url_encode(const char *src, char *dst, size_t dst_len) {
  static const char *dont_escape = "/._-$,;~()";
  static const char *hex = "0123456789abcdef";
  const char *end = dst + dst_len - 1;

  for (; *src != '\0' && dst < end; src++, dst++) {
    if (isalnum(*(const unsigned char *) src) ||
        strchr(dont_escape, * (const unsigned char *) src) != NULL) {
      *dst = *src;
    } else if (dst + 2 < end) {
      dst[0] = '%';
      dst[1] = hex[(* (const unsigned char *) src) >> 4];
      dst[2] = hex[(* (const unsigned char *) src) & 0xf];
      dst += 2;
    }
  }

  *dst = '\0';
}
