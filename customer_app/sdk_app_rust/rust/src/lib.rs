//!  Main Rust Application for BL602 Firmware
#![no_std]  //  Use the Rust Core Library instead of the Rust Standard Library, which is not compatible with embedded systems

//  Import the Rust Core Library
use core::{
    panic::PanicInfo,  //  For `PanicInfo` type used by `panic` function
    str::FromStr,      //  For converting `str` to `String`
};

/// `rust_main` will be called by the BL602 command-line interface
#[no_mangle]              //  Don't mangle the name `rust_main`
extern "C" fn rust_main(  //  Declare `extern "C"` because it will be called by BL602 firmware
    _buf:  *const u8,        //  Command line (char *)
    _len:  i32,              //  Length of command line (int)
    _argc: i32,              //  Number of command line args (int)
    _argv: *const *const u8  //  Array of command line args (char **)
) {
    //  Show a message on the serial console
    puts("Hello from Rust!");

    //  PineCone Blue LED is connected on BL602 GPIO 11
    const LED_GPIO: u8 = 11;  //  `u8` is 8-bit unsigned integer

    //  Configure the LED GPIO for output (instead of input)
    bl_gpio_enable_output(LED_GPIO, 0, 0)      //  No pullup, no pulldown
        .expect("GPIO enable output failed");  //  Halt on error

    //  Blink the LED 5 times
    for i in 0..10 {  //  Iterates 10 times from 0 to 9 (`..` excludes 10)

        //  Toggle the LED GPIO between 0 (on) and 1 (off)
        bl_gpio_output_set(  //  Set the GPIO output (from BL602 GPIO HAL)
            LED_GPIO,        //  GPIO pin number
            i % 2            //  0 for low, 1 for high
        ).expect("GPIO output failed");  //  Halt on error

        //  Sleep 1 second
        time_delay(                   //  Sleep by number of ticks (from NimBLE Porting Layer)
            time_ms_to_ticks32(1000)  //  Convert 1,000 milliseconds to ticks (from NimBLE Porting Layer)
        );
    }

    //  Return to the BL602 command-line interface
}

/// This function is called on panic, like an assertion failure
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {  //  `!` means that panic handler will never return
    //  TODO: Implement the complete panic handler like this:
    //  https://github.com/lupyuen/pinetime-rust-mynewt/blob/master/rust/app/src/lib.rs#L115-L146

    //  For now we display a message
    puts("TODO: Rust panic"); 

	//  Loop forever, do not pass go, do not collect $200
    loop {}
}

///////////////////////////////////////////////////////////////////////////////
//  Safe Wrappers for BL602 C Functions imported from
//  BL602 IoT SDK and NimBLE Porting Layer
//  (Will be auto-generated by `bindgen`)

/// Print a message to the serial console.
/// TODO: Auto-generate this wrapper with `bindgen` from the C declaration
fn puts(s: &str) -> i32 {  //  `&str` is a reference to a string slice, similar to `char *` in C

    extern "C" {  //  Import C Function
        /// Print a message to the serial console (from C stdio library)
        fn puts(s: *const u8) -> i32;
    }

    //  Convert `str` to `String`, which similar to `char [64]` in C
    let mut s_with_null = String::from_str(s)  //  `mut` because we will modify it
        .expect("puts conversion failed");     //  If it exceeds 64 chars, halt with an error
    
    //  Terminate the string with null, since we will be passing to C
    s_with_null.push('\0')
        .expect("puts overflow");  //  If we exceed 64 chars, halt with an error

    //  Convert the null-terminated string to a pointer
    let p = s_with_null.as_str().as_ptr();

    //  Call the C function
    unsafe {  //  Flag this code as unsafe because we're calling a C function
        puts(p)
    }

    //  No semicolon `;` here, so the value returned by the C function will be passed to our caller
}

/// Configure a GPIO pin for output (instead of input).
/// TODO: Auto-generate this wrapper with `bindgen` from the C declaration:
/// `int bl_gpio_enable_output(uint8_t pin, uint8_t pullup, uint8_t pulldown)`
fn bl_gpio_enable_output(
    pin:      u8,  //  GPIO pin number (uint8_t)
    pullup:   u8,  //  0 for no pullup, 1 for pullup (uint8_t) 
    pulldown: u8   //  0 for no pulldown, 1 for pulldown (uint8_t)
) -> Result<(), i32> {  //  Returns an error code (int)

    extern "C" {        //  Import C Function
        /// Configure a GPIO pin for output (from BL602 GPIO HAL)
        fn bl_gpio_enable_output(pin: u8, pullup: u8, pulldown: u8) -> i32;
    }

    //  Call the C function
    let res = unsafe {  //  Flag this code as unsafe because we're calling a C function
        bl_gpio_enable_output(pin, pullup, pulldown)
    };

    //  Check the result code
    match res {
        0 => Ok(()),   //  If no error, return OK
        _ => Err(res)  //  Else return the result code as an error
    }
}

/// Set the GPIO pin output to high or low.
/// TODO: Auto-generate this wrapper with `bindgen` from the C declaration:
/// `int bl_gpio_output_set(uint8_t pin, uint8_t value)`
fn bl_gpio_output_set(
    pin:   u8,  //  GPIO pin number (uint8_t)
    value: u8   //  0 for low, 1 to high
) -> Result<(), i32> {  //  Returns an error code (int)

    extern "C" {        //  Import C Function
        /// Set the GPIO pin output to high or low (from BL602 GPIO HAL)
        fn bl_gpio_output_set(pin: u8, value: u8) -> i32;
    }

    //  Call the C function
    let res = unsafe {  //  Flag this code as unsafe because we're calling a C function
        bl_gpio_output_set(pin, value)
    };

    //  Check the result code
    match res {
        0 => Ok(()),   //  If no error, return OK
        _ => Err(res)  //  Else return the result code as an error
    }
}

/// Convert milliseconds to system ticks.
/// TODO: Auto-generate this wrapper with `bindgen` from the C declaration:
/// `ble_npl_time_t ble_npl_time_ms_to_ticks32(uint32_t ms)`
fn time_ms_to_ticks32(
    ms: u32  //  Number of milliseconds
) -> u32 {   //  Returns the number of ticks (uint32_t)
    extern "C" {        //  Import C Function
        /// Convert milliseconds to system ticks (from NimBLE Porting Layer)
        fn ble_npl_time_ms_to_ticks32(ms: u32) -> u32;
    }

    //  Call the C function
    unsafe {  //  Flag this code as unsafe because we're calling a C function
        ble_npl_time_ms_to_ticks32(ms)
    }

    //  No semicolon `;` here, so the value returned by the C function will be passed to our caller
}

/// Sleep for the specified number of system ticks.
/// TODO: Auto-generate this wrapper with `bindgen` from the C declaration:
/// `void ble_npl_time_delay(ble_npl_time_t ticks)`
fn time_delay(
    ticks: u32  //  Number of ticks to sleep
) {
    extern "C" {  //  Import C Function
        /// Sleep for the specified number of system ticks (from NimBLE Porting Layer)
        fn ble_npl_time_delay(ticks: u32);
    }

    //  Call the C function
    unsafe {  //  Flag this code as unsafe because we're calling a C function
        ble_npl_time_delay(ticks);
    }
}

/// Limit Strings to 64 chars, similar to `char[64]` in C
type String = heapless::String::<heapless::consts::U64>;

/* Output Log

# help
====Build-in Commands====
====Support 4 cmds once, seperate by ; ====
help                     : print this
p                        : print memory
m                        : modify memory
echo                     : echo for command
exit                     : close CLI
devname                  : print device name
sysver                   : system version
reboot                   : reboot system
poweroff                 : poweroff system
reset                    : system reset
time                     : system time
ota                      : system ota
ps                       : thread dump
ls                       : file list
hexdump                  : dump file
cat                      : cat file

====User Commands====
rust_main                : Run Rust code
blogset                  : blog pri set level
blogdump                 : blog info dump
bl_sys_time_now          : sys time now

# rust_main
Hello from Rust!

# rust_main
Hello from Rust!

# rust_main
Hello from Rust!

*/