use std::{io, env, fs};

fn main() {
    let git_sha = read_git_sha().unwrap();
    println!("cargo:rustc-env={}={}", "BUILD_SHA", git_sha);

    println!(
        "cargo:rustc-env=BUILD_TARGET={}",
        std::env::var("TARGET").unwrap()
    );
}

fn read_git_sha() -> io::Result<String> {
    let git_path = env::current_dir().unwrap().parent().unwrap().join(".git");
    let git_head_path = git_path.join("HEAD");
    println!("cargo:rerun-if-changed={}", git_head_path.to_str().unwrap());
    let mut head_content = fs::read_to_string(&git_head_path)?;
    assert!(head_content.ends_with("\n"));
    head_content.pop();

    if head_content.starts_with("ref: ") {
        // We're on a branch. Read the SHA from the ref file.
        head_content.replace_range(0.."ref: ".len(), "");
        let ref_filename = git_path.join(&head_content);
        println!("cargo:rerun-if-changed={}", ref_filename.to_str().unwrap());
        fs::read_to_string(&ref_filename)
    } else {
        // We're not on a branch. The `HEAD` file itself contains the sha.
        assert_eq!(head_content.len(), 40);
        Ok(head_content)
    }
}
