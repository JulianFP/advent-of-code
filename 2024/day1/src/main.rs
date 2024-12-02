use std::env;
use std::fs::File;
use std::io::Read;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide a filename as a cli argument");
    }
    let mut file = File::open(&args[1]).expect("provided file could not be opened");
    let mut contents = String::new();
    file.read_to_string(&mut contents).expect("provided file could not be read");

    let mut first_array = Vec::new();
    let mut second_array = Vec::new();
    for line in contents.lines() {
        let line_splitted: Vec<&str> = line.split("   ").collect();
        if line_splitted.len() != 2 {
            panic!("Every file line is supposed to have only 2 numbers");
        };
        first_array.push(line_splitted[0].parse::<i32>().expect("provided file has wrong format"));
        second_array.push(line_splitted[1].parse::<i32>().expect("provided file has wrong format"));
    }

    first_array.sort();
    second_array.sort();
    let mut diff_sum = 0;
    for pair in first_array.iter().zip(second_array.iter()) {
        diff_sum += (pair.0 - pair.1).abs();
    }
    println!("The solution for the first part is {diff_sum}");


    let mut similarity_score = 0;
    let mut dup_counter_left: i32;
    let mut dup_counter_right: i32;
    let mut left_curr_pos: usize = 0;
    let mut left_last_pos: usize;
    let mut right_curr_pos: usize = 0;
    let mut right_last_pos: usize;
    let mut over = false;
    while !over {
        while first_array[left_curr_pos] != second_array[right_curr_pos] && !over {
            while first_array[left_curr_pos] < second_array[right_curr_pos] {
                if left_curr_pos+1 >= first_array.len() {
                    over = true;
                    break;
                }
                left_curr_pos += 1;
            }
            while first_array[left_curr_pos] > second_array[right_curr_pos] {
                if right_curr_pos+1 >= second_array.len() {
                    over = true;
                    break;
                }
                right_curr_pos += 1;
            }
        }
        if over {
            break;
        }

        //iterate left side until not the same value anymore
        left_last_pos = left_curr_pos;
        dup_counter_left = 0;
        while first_array[left_curr_pos] == first_array[left_last_pos] {
            dup_counter_left += 1;
            if left_curr_pos+1 >= first_array.len() {
                over = true;
                break;
            }
            left_last_pos = left_curr_pos;
            left_curr_pos += 1;
        }

        //do the same for right side
        right_last_pos = right_curr_pos;
        dup_counter_right = 0;
        while second_array[right_curr_pos] == second_array[right_last_pos] {
            dup_counter_right += 1;
            if right_curr_pos+1 >= second_array.len() {
                over = true;
                break;
            }
            right_last_pos = right_curr_pos;
            right_curr_pos += 1;
        }

        similarity_score += dup_counter_left * dup_counter_right * first_array[left_last_pos];
    }

    println!("The solution for the second part is {similarity_score}");
}
