use std::env;
use std::fs::File;
use std::io::Read;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide a filename as a cli argument");
    }
    let mut file = File::open(&args[1]).expect("Provided file could not be opened");
    let mut contents = String::new();
    file.read_to_string(&mut contents).expect("Provided file could not be read");

    let mut count = 0;
    let lines: Vec<&str> = contents.lines().collect();
    //first parse every row forwards and backwards
    for (row_i, row) in lines.iter().enumerate() {
        for (col_i, char) in row.chars().enumerate() {
            //scan for start of word 'XMAS'
            if char == 'X' {
                let left = col_i > 2;
                let right = col_i < row.len() - 3;
                let up = row_i > 2;
                let down = row_i < lines.len() - 3;
                if left {
                    if row[col_i-3..=col_i].eq("SAMX") {
                        count += 1;
                    }
                }
                if right {
                    if row[col_i..=col_i+3].eq("XMAS") {
                        count += 1;
                    }
                }
                if up {
                    if lines[row_i-1].chars().nth(col_i).unwrap() == 'M' && lines[row_i-2].chars().nth(col_i).unwrap() == 'A' && lines[row_i-3].chars().nth(col_i).unwrap() == 'S' {
                        count += 1;
                    }
                }
                if down {
                    if lines[row_i+1].chars().nth(col_i).unwrap() == 'M' && lines[row_i+2].chars().nth(col_i).unwrap() == 'A' && lines[row_i+3].chars().nth(col_i).unwrap() == 'S' {
                        count += 1;
                    }
                }
                if left && up {
                    if lines[row_i-1].chars().nth(col_i-1).unwrap() == 'M' && lines[row_i-2].chars().nth(col_i-2).unwrap() == 'A' && lines[row_i-3].chars().nth(col_i-3).unwrap() == 'S' {
                        count += 1;
                    }
                }
                if left && down {
                    if lines[row_i+1].chars().nth(col_i-1).unwrap() == 'M' && lines[row_i+2].chars().nth(col_i-2).unwrap() == 'A' && lines[row_i+3].chars().nth(col_i-3).unwrap() == 'S' {
                        count += 1;
                    }
                }
                if right && up {
                    if lines[row_i-1].chars().nth(col_i+1).unwrap() == 'M' && lines[row_i-2].chars().nth(col_i+2).unwrap() == 'A' && lines[row_i-3].chars().nth(col_i+3).unwrap() == 'S' {
                        count += 1;
                    }
                }
                if right && down {
                    if lines[row_i+1].chars().nth(col_i+1).unwrap() == 'M' && lines[row_i+2].chars().nth(col_i+2).unwrap() == 'A' && lines[row_i+3].chars().nth(col_i+3).unwrap() == 'S' {
                        count += 1;
                    }
                }
            }
        }
    }
    println!("The solution for the first part is {count}");


    count = 0;
    for (row_i, row) in lines.iter().enumerate() {
        for (col_i, char) in row.chars().enumerate() {
            //scan for A's since they are always in the middle of an X-MAS
            if char == 'A' {
                //check if there is enough space in all directions
                if col_i > 0 && row_i > 0 && col_i < row.len() - 1 && row_i < lines.len() - 1 {
                    if ((lines[row_i-1].chars().nth(col_i-1).unwrap() == 'M' && lines[row_i+1].chars().nth(col_i+1).unwrap() == 'S')
                    || (lines[row_i-1].chars().nth(col_i-1).unwrap() == 'S' && lines[row_i+1].chars().nth(col_i+1).unwrap() == 'M'))
                    && 
                    ((lines[row_i-1].chars().nth(col_i+1).unwrap() == 'M' && lines[row_i+1].chars().nth(col_i-1).unwrap() == 'S')
                    || (lines[row_i-1].chars().nth(col_i+1).unwrap() == 'S' && lines[row_i+1].chars().nth(col_i-1).unwrap() == 'M')) {
                        count += 1;
                    }
                }
            }
        }
    }
    println!("The solution for the second part is {count}");
}
