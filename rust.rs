#![feature(asm)]

use libc::{mmap, munmap, MAP_ANON, MAP_PRIVATE, PROT_READ, PROT_WRITE};
use rand::Rng;
use std::mem;
use std::os::raw::c_void;
use std::ptr;

const SIZE: usize = 10;

fn main() {
    // Advanced Memory Manipulation with mmap
    unsafe {
        let mem_block = mmap(
            std::ptr::null_mut(),
            SIZE * mem::size_of::<i32>(),
            PROT_READ | PROT_WRITE,
            MAP_ANON | MAP_PRIVATE,
            -1,
            0,
        ) as *mut i32;

        if mem_block.is_null() {
            eprintln!("Failed to allocate memory");
            return;
        }

        // Manipulating memory
        for i in 0..SIZE {
            ptr::write(mem_block.add(i), i as i32);
        }

        // Reading values
        for i in 0..SIZE {
            println!("Value at position {}: {}", i, ptr::read(mem_block.add(i)));
        }

        // Deallocating memory
        munmap(mem_block as *mut c_void, SIZE * mem::size_of::<i32>());
    }

    // Inline Assembly (x86_64 specific)
    #[cfg(target_arch = "x86_64")]
    unsafe {
        let mut output: u64;
        asm!(
            "mov rax, 0x0",
            "cpuid",
            out("rax") output,
            out("rbx") _, out("rcx") _, out("rdx") _,
        );
        println!("CPUID result: {}", output);
    }

    // Generate a random number
    let mut rng = rand::thread_rng();
    let random_number: i32 = rng.gen();
    println!("Random number: {}", random_number);
}
