#include <minix/drivers.h>
#include <minix/chardriver.h>
#include <sys/ioc_homework.h>
#include <stdio.h>
#include <stdlib.h>
#include <minix/ds.h>
#include "homework.h"

/*
 * Function prototypes for the homework driver.
 */
static int homework_open(devminor_t minor, int access, endpoint_t user_endpt);
static int homework_close(devminor_t minor);
static ssize_t homework_read(devminor_t minor, u64_t position, endpoint_t endpt,
    cp_grant_id_t grant, size_t size, int flags, cdev_id_t id);
static ssize_t homework_write(devminor_t minor, u64_t position, endpoint_t endpt,
	cp_grant_id_t grant, size_t size, int UNUSED(flags),
	cdev_id_t UNUSED(id));
static int homework_ioctl(devminor_t minor, unsigned long request, endpoint_t endpt,
	cp_grant_id_t grant, int flags, endpoint_t user_endpt, cdev_id_t id);

/* SEF functions and variables. */
static void sef_local_startup(void);
static int sef_cb_init(int type, sef_init_info_t *info);
static int sef_cb_lu_state_save(int);
static int lu_state_restore(void);

/* Entry points to the homework driver. */
static struct chardriver homework_tab =
{
    .cdr_open	= homework_open,
    .cdr_close	= homework_close,
    .cdr_read	= homework_read,
    .cdr_write	= homework_write,
    .cdr_ioctl  = homework_ioctl
};

/** State variable to count the number of times the device has been opened.
 * Note that this is not the regular type of open counter: it never decreases.
 */
static int open_counter;
/** State variable to store an integer written to this pseudo-driver
 * Note that this is an array of 4 integers and is initialized to 0
 */
static u32_t slots[5] = { 0 };
/** State variable to store which integer slot is currently available for reading
 * Note that this integer is initialized to 0
 */
static u32_t slot_in_use = 0;
// State variable to store size of reads and writes
const static size_t integer_size = sizeof(slot_in_use);

/*===========================================================================*
 *    homework_open                                                     *
 *    -Print opening, do no setup
 *===========================================================================*/
static int homework_open(devminor_t UNUSED(minor), int UNUSED(access),
    endpoint_t UNUSED(user_endpt))
{
    printf("(Driver) homework_open(). Called %d time(s).\n", ++open_counter);
    return OK;
}

/*===========================================================================*
 *    homework_close                                                     *
 *    -Print closing, do no cleanup
 *===========================================================================*/
static int homework_close(devminor_t UNUSED(minor))
{
    printf("(Driver) homework_close()\n");
    return OK;
}

/*===========================================================================*
 *    homework_read                                                     *
 *    -Print reading, return integer in slot available
 *===========================================================================*/
static ssize_t homework_read(devminor_t UNUSED(minor), u64_t UNUSED(position),
    endpoint_t endpt, cp_grant_id_t grant, size_t size, int UNUSED(flags),
    cdev_id_t UNUSED(id))
{
    printf("(Driver) homework_read()\n");

    u32_t *ptr = slots + slot_in_use;
    int ret;
    //char *buf = HOMEWORK_MESSAGE;
    if (size < 4)
    { 
            printf("(Driver) homework_read(): Read MUST be 4 bytes\n");
            return EINVAL;
    }

    /* Copy the requested part to the caller. */
    ret = sys_safecopyto(endpt, grant, 0, (vir_bytes) ptr, integer_size);
    return (ret != OK) ? ret : integer_size;
}

/*===========================================================================*
 *    homework_write                                                     *
 *    -Print writing, write integer to slot available
 *===========================================================================*/
static ssize_t homework_write(devminor_t minor, u64_t position, endpoint_t endpt,
	cp_grant_id_t grant, size_t size, int UNUSED(flags),
	cdev_id_t UNUSED(id))
	{
    printf("(Driver) homework_write()\n");

    int ret;
    u32_t *ptr = slots + slot_in_use;

    //char *buf = HOMEWORK_MESSAGE;
    if (size < 4)
    { 
            printf("(Driver) homework_write(): Write MUST be 4 bytes\n");
            return EINVAL;
    }

    /* Copy the requested part from the caller to the device */
    ret = sys_safecopyfrom(endpt, grant, 0, (vir_bytes) ptr, integer_size);
    return (ret != OK) ? ret : integer_size;
}

/*===========================================================================*
 *    homework_ioctl                                                     *
 *    -Print ioctl_fcn called (HIOCSLOT, HIOCCSLOT, HIOCGETSLOT), change, clear, or get available slot
 *===========================================================================*/
static int homework_ioctl(devminor_t minor, unsigned long request, endpoint_t endpt,
	cp_grant_id_t grant, int flags, endpoint_t user_endpt, cdev_id_t id)
{
        u32_t tmp_slot = slot_in_use;
        switch (request) {
                case HIOCSLOT:
                        printf("(Driver) homework_ioctl() HIOCSLOT\n");
                        /* Set input to slot value */
                        sys_safecopyfrom(endpt, grant, 0, (vir_bytes) &tmp_slot, integer_size);
                        if (tmp_slot > 4)
                        {
                                printf("(Driver) homework_HIOCSLOT(): Slot must be 0-4\n");
                                return EINVAL;
                        }
                        slot_in_use = tmp_slot;
                        printf("(Driver) slot in use = %d\n", slot_in_use);
                        return EXIT_SUCCESS;
                case HIOCCLEARSLOT:
                        printf("(Driver) homework_ioctl() HIOCCLEARSLOT\n");
                        /* Clear input slot (set to 0) */
                        sys_safecopyfrom(endpt, grant, 0, (vir_bytes) &tmp_slot, integer_size);
                        if (tmp_slot > 4)
                        {
                                printf("(Driver) homework_HIOCSLOT(): Slot must be 0-4\n");
                                return EINVAL;
                        }
                        slots[tmp_slot] = 0;
                        printf("(Driver) slot cleared(%d) value = %d\n", tmp_slot, slots[tmp_slot]);
                        return EXIT_SUCCESS;
                case HIOCGETSLOT:
                        printf("(Driver) homework_ioctl() HIOCGETSLOT\n");
                        /* Return the current slot_in_use */
                        sys_safecopyto(endpt, grant, 0, (vir_bytes) &slot_in_use, integer_size);
                        printf("(Driver) slot returned: = %d\n", tmp_slot);
                        return EXIT_SUCCESS;
                default:
                        break;
        }

        return ENOTTY;
}

static int sef_cb_lu_state_save(int UNUSED(state)) {
        /* Save the state. */
        ds_publish_u32("open_counter", open_counter, DSF_OVERWRITE);

        return OK;
}

static int lu_state_restore() {
/* Restore the state. */
    u32_t value;

    ds_retrieve_u32("open_counter", &value);
    ds_delete_u32("open_counter");
    open_counter = (int) value;

    return OK;
}

static void sef_local_startup()
{
    /*
     * Register init callbacks. Use the same function for all event types
     */
    sef_setcb_init_fresh(sef_cb_init);
    sef_setcb_init_lu(sef_cb_init);
    sef_setcb_init_restart(sef_cb_init);

    /*
     * Register live update callbacks.
     */
    /* - Agree to update immediately when LU is requested in a valid state. */
    sef_setcb_lu_prepare(sef_cb_lu_prepare_always_ready);
    /* - Support live update starting from any standard state. */
    sef_setcb_lu_state_isvalid(sef_cb_lu_state_isvalid_standard);
    /* - Register a custom routine to save the state. */
    sef_setcb_lu_state_save(sef_cb_lu_state_save);

    /* Let SEF perform startup. */
    sef_startup();
}

static int sef_cb_init(int type, sef_init_info_t *UNUSED(info))
{
/* Initialize the homework driver. */
    int do_announce_driver = TRUE;

    open_counter = 0;
    switch(type) {
        case SEF_INIT_FRESH:
            printf("(Driver) %s", HOMEWORK_MESSAGE);
        break;

        case SEF_INIT_LU:
            /* Restore the state. */
            lu_state_restore();
            do_announce_driver = FALSE;

            printf("(Driver) %sHey, I'm a new version!\n", HOMEWORK_MESSAGE);
        break;

        case SEF_INIT_RESTART:
            printf("(Driver) %sHey, I've just been restarted!\n", HOMEWORK_MESSAGE);
        break;
    }

    /* Announce we are up when necessary. */
    if (do_announce_driver) {
        chardriver_announce();
    }

    /* Initialization completed successfully. */
    return OK;
}

int main(void)
{
    /*
     * Perform initialization.
     */
    sef_local_startup();

    /*
     * Run the main loop.
     */
    chardriver_task(&homework_tab);
    return OK;
}

