# R script to convert R markdown to LaTeX
# (used by OpenFLUID docalyzer)

args <- commandArgs(trailingOnly = TRUE)

library(knitr)
library(rmarkdown)

knitr::knit(args[1], output = args[2])

if (file.exists(args[2])) {

  # use pandoc to produce LaTeX file,
  # with the "--citeproc" option to manage bibliography
  rmarkdown::pandoc_convert(args[2], to = "latex",
                            output = args[3],
                            citeproc = TRUE)
  quit(status = (!file.exists(args[3])))

} else {

  quit(status = 1)

}
