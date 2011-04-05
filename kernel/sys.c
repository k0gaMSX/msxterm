
#include "tnix/sys.h"
#include "tnix/vt.h"
#include "tnix/kernel.h"
#include "arch/arch.h"

#define NR_INT_ACTIONS   5



static struct int_action {
     int_handler handler;       /* interrupt handler */
     void *data;                /* data required by the handler */
} int_actions [NR_INT_ACTIONS];



void sys_init(void)
{
     con_init();
     printk("Booting tnix");
}



/*
 * This routine runs over registered interrupt handlers asking them about
 * the interrupt. The first handler returns non zero means it takes the
 * interrupt.
 */


void int_dispatch(void)
{
     register struct int_action *bp;

     for (bp = int_actions; bp < int_actions + NR_INT_ACTIONS; ++bp) {
          if (!bp->handler) {      /* it is not used, so there isn't more */
               printk("Unexpected interrupt\n");              /* handlers */
               break;
          }

          if ((*bp->handler)(bp->data))
               break;     /* this handler takes the interrupt */
     }
}


/*
 * Store interrupt handlers and  pointer to the data needed by the handler.
 */

void request_irq(int_handler handler, void *data)
{
     register struct int_action *bp;

     for (bp = int_actions; bp < int_actions + NR_INT_ACTIONS; ++bp) {
          if (!bp->handler) {
               bp->handler = handler;
               bp->data = data;
               return;
          }
     }

     panic("Requested too much irqs");
}
