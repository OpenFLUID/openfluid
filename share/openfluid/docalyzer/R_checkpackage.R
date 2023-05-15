# R script to check if a package is correctly installed
# (used by OpenFLUID docalyzer)

args <- commandArgs(trailingOnly = TRUE)

package_path <- system.file(package = args[1])

if ((package_path == "")) {
  cat("package", args[1], "not found")
  quit(status = 1)
} else {
  cat("package", args[1], "found")
  quit(status = 0)
}
