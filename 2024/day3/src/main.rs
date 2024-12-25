use std::env;
use std::fs::File;
use std::io::Read;
use regex::Regex;

fn parse_mul(input_string: &str) -> Vec<(i32, i32)>{
    let re = Regex::new(r"mul\((\d{1,3}),(\d{1,3})\)").unwrap();
    re.captures_iter(input_string).map(|caps| {
        let (_, [first,second]) = caps.extract();
        (first.parse::<i32>().unwrap(),second.parse::<i32>().unwrap())
    }).collect()
}

fn split_into_valid_parts(input_string: &str) -> Vec<&str>{
    let mut result = Vec::new();
    let mut current_string = input_string;
    loop {
        match current_string.split_once(r"don't()") {
            None => {
                result.push(current_string);
                return result;
            },
            Some((valid, next)) => {
                result.push(valid);
                current_string = next;
            }
        }
        match current_string.split_once(r"do()") {
            None => return result,
            Some((_, next)) => {
                current_string = next;
            }
        }
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide a filename as a cli argument");
    }
    let mut file = File::open(&args[1]).expect("Provided file could not be opened");
    let mut contents = String::new();
    file.read_to_string(&mut contents).expect("Provided file could not be read");

    let mut result = 0;
    for mul in parse_mul(&contents) {
        let (first,second) = mul;
        result += first * second;
    }
    println!("The solution for the first part is {result}");

    result = 0;
    for part in split_into_valid_parts(&contents) {
        for mul in parse_mul(&part) {
            let (first,second) = mul;
            result += first * second;
        }
    }
    println!("The solution for the second part is {result}");
}
