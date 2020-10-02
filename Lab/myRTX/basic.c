#include "MKL25Z4.h"                    // Device header
#include "basic.h"


uint32_t getMaskPort(PORT_Type *port) {
  if (port == PORTA)
    return SIM_SCGC5_PORTA_MASK;
  else if (port == PORTB)
    return SIM_SCGC5_PORTB_MASK;
  else if (port == PORTC)
    return SIM_SCGC5_PORTC_MASK;
  else if (port == PORTD)
    return SIM_SCGC5_PORTD_MASK;
  else
    return SIM_SCGC5_PORTE_MASK;
}
