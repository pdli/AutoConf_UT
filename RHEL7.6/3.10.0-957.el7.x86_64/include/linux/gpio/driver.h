#ifndef __LINUX_GPIO_DRIVER_H
#define __LINUX_GPIO_DRIVER_H

#include <linux/device.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/irq.h>
#include <linux/irqchip/chained_irq.h>
#include <linux/irqdomain.h>
#include <linux/pinctrl/pinctrl.h>
#include <linux/pinctrl/pinconf-generic.h>

struct gpio_desc;
struct of_phandle_args;
struct device_node;
struct seq_file;
struct gpio_device;

/**
 * struct gpio_chip - abstract a GPIO controller
 * @label: for diagnostics
 * @gpiodev: the internal state holder, opaque struct
 * @dev: optional device providing the GPIOs
 * @owner: helps prevent removal of modules exporting active GPIOs
 * @request: optional hook for chip-specific activation, such as
 *	enabling module power and clock; may sleep
 * @free: optional hook for chip-specific deactivation, such as
 *	disabling module power and clock; may sleep
 * @get_direction: returns direction for signal "offset", 0=out, 1=in,
 *	(same as GPIOF_DIR_XXX), or negative error
 * @direction_input: configures signal "offset" as input, or returns error
 * @direction_output: configures signal "offset" as output, or returns error
 * @get: returns value for signal "offset"; for output signals this
 *	returns either the value actually sensed, or zero
 * @set: assigns output value for signal "offset"
 * @set_config: optional hook for all kinds of settings. Uses the same
 *	packed config format as generic pinconf.
 * @to_irq: optional hook supporting non-static gpio_to_irq() mappings;
 *	implementation may not sleep
 * @dbg_show: optional routine to show contents in debugfs; default code
 *	will be used when this is omitted, but custom code can show extra
 *	state (such as pullup/pulldown configuration).
 * @base: identifies the first GPIO number handled by this chip; or, if
 *	negative during registration, requests dynamic ID allocation.
 * @ngpio: the number of GPIOs handled by this controller; the last GPIO
 *	handled is (base + ngpio - 1).
 * @desc: array of ngpio descriptors. Private.
 * @names: if set, must be an array of strings to use as alternative
 *      names for the GPIOs in this chip. Any entry in the array
 *      may be NULL if there is no alias for the GPIO, however the
 *      array must be @ngpio entries long.  A name can include a single printk
 *      format specifier for an unsigned int.  It is substituted by the actual
 *      number of the gpio.
 * @can_sleep: flag must be set iff get()/set() methods sleep, as they
 *	must while accessing GPIO expander chips over I2C or SPI. This
 *	implies that if the chip supports IRQs, these IRQs need to be threaded
 *	as the chip access may sleep when e.g. reading out the IRQ status
 *	registers.
 * @exported: flags if the gpiochip is exported for use from sysfs. Private.
 *
 * A gpio_chip can help platforms abstract various sources of GPIOs so
 * they can all be accessed through a common programing interface.
 * Example sources would be SOC controllers, FPGAs, multifunction
 * chips, dedicated GPIO expanders, and so on.
 *
 * Each chip controls a number of signals, identified in method calls
 * by "offset" values in the range 0..(@ngpio - 1).  When those signals
 * are referenced through calls like gpio_get_value(gpio), the offset
 * is calculated by subtracting @base from the gpio number.
 */
struct gpio_chip {
	const char		*label;
	struct gpio_device	*gpiodev;
	struct device		*dev;
	struct module		*owner;
	void			*data;

	int			(*request)(struct gpio_chip *chip,
						unsigned offset);
	void			(*free)(struct gpio_chip *chip,
						unsigned offset);
	int			(*get_direction)(struct gpio_chip *chip,
						unsigned offset);
	int			(*direction_input)(struct gpio_chip *chip,
						unsigned offset);
	int			(*direction_output)(struct gpio_chip *chip,
						unsigned offset, int value);
	int			(*get)(struct gpio_chip *chip,
						unsigned offset);
	void			(*set)(struct gpio_chip *chip,
						unsigned offset, int value);
	int			(*set_config)(struct gpio_chip *chip,
					      unsigned offset,
					      unsigned long config);
	int			(*to_irq)(struct gpio_chip *chip,
						unsigned offset);

	void			(*dbg_show)(struct seq_file *s,
						struct gpio_chip *chip);
	int			base;
	u16			ngpio;
	struct gpio_desc	*desc;
	const char		*const *names;
	bool			can_sleep;
	bool			exported;

#ifdef CONFIG_GPIOLIB_IRQCHIP
	/*
	 * With CONFIG_GPIO_IRQCHIP we get an irqchip inside the gpiolib
	 * to handle IRQs for most practical cases.
	 */
	struct irq_chip		*irqchip;
	struct irq_domain	*irqdomain;
	unsigned int		irq_base;
	irq_flow_handler_t	irq_handler;
	unsigned int		irq_default_type;
#endif

#if defined(CONFIG_OF_GPIO)
	/*
	 * If CONFIG_OF is enabled, then all GPIO controllers described in the
	 * device tree automatically may have an OF translation
	 */
	struct device_node *of_node;
	int of_gpio_n_cells;
	int (*of_xlate)(struct gpio_chip *gc,
			const struct of_phandle_args *gpiospec, u32 *flags);
#endif
#ifdef CONFIG_PINCTRL
	/*
	 * If CONFIG_PINCTRL is enabled, then gpio controllers can optionally
	 * describe the actual pin range which they serve in an SoC. This
	 * information would be used by pinctrl subsystem to configure
	 * corresponding pins for gpio usage.
	 */
	struct list_head pin_ranges;
#endif
};

extern const char *gpiochip_is_requested(struct gpio_chip *chip,
			unsigned offset);

/* add/remove chips */
extern int gpiochip_add_data(struct gpio_chip *chip, void *data);
static inline int gpiochip_add(struct gpio_chip *chip)
{
	return gpiochip_add_data(chip, NULL);
}
extern int gpiochip_remove(struct gpio_chip *chip);
extern struct gpio_chip *gpiochip_find(void *data,
			      int (*match)(struct gpio_chip *chip, void *data));

/* lock/unlock as IRQ */
int gpiod_lock_as_irq(struct gpio_desc *desc);
void gpiod_unlock_as_irq(struct gpio_desc *desc);
bool gpiochip_line_is_irq(struct gpio_chip *chip, unsigned int offset);

/* get driver data */
static inline void *gpiochip_get_data(struct gpio_chip *chip)
{
	return chip->data;
}

enum gpio_lookup_flags {
	GPIO_ACTIVE_HIGH = (0 << 0),
	GPIO_ACTIVE_LOW = (1 << 0),
	GPIO_OPEN_DRAIN = (1 << 1),
	GPIO_OPEN_SOURCE = (1 << 2),
};

/**
 * struct gpiod_lookup - lookup table
 * @chip_label: name of the chip the GPIO belongs to
 * @chip_hwnum: hardware number (i.e. relative to the chip) of the GPIO
 * @con_id: name of the GPIO from the device's point of view
 * @idx: index of the GPIO in case several GPIOs share the same name
 * @flags: mask of GPIO_* values
 *
 * gpiod_lookup is a lookup table for associating GPIOs to specific devices and
 * functions using platform data.
 */
struct gpiod_lookup {
	const char *chip_label;
	u16 chip_hwnum;
	const char *con_id;
	unsigned int idx;
	enum gpio_lookup_flags flags;
};

struct gpiod_lookup_table {
	struct list_head list;
	const char *dev_id;
	struct gpiod_lookup table[];
};

/*
 * Simple definition of a single GPIO under a con_id
 */
#define GPIO_LOOKUP(_chip_label, _chip_hwnum, _con_id, _flags) \
	GPIO_LOOKUP_IDX(_chip_label, _chip_hwnum, _con_id, 0, _flags)

/*
 * Use this macro if you need to have several GPIOs under the same con_id.
 * Each GPIO needs to use a different index and can be accessed using
 * gpiod_get_index()
 */
#define GPIO_LOOKUP_IDX(_chip_label, _chip_hwnum, _con_id, _idx, _flags)  \
{                                                                         \
	.chip_label = _chip_label,                                        \
	.chip_hwnum = _chip_hwnum,                                        \
	.con_id = _con_id,                                                \
	.idx = _idx,                                                      \
	.flags = _flags,                                                  \
}

void gpiod_add_lookup_table(struct gpiod_lookup_table *table);

#ifdef CONFIG_GPIOLIB_IRQCHIP

void gpiochip_set_chained_irqchip(struct gpio_chip *gpiochip,
		struct irq_chip *irqchip,
		int parent_irq,
		irq_flow_handler_t parent_handler);

int gpiochip_irqchip_add(struct gpio_chip *gpiochip,
		struct irq_chip *irqchip,
		unsigned int first_irq,
		irq_flow_handler_t handler,
		unsigned int type);

#endif /* CONFIG_GPIO_IRQCHIP */

int gpiochip_generic_config(struct gpio_chip *chip, unsigned offset,
			    unsigned long config);

#endif
