% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/GatingSet_Methods.R
\docType{methods}
\name{lapply,GatingSet-method}
\alias{lapply,GatingSet-method}
\title{apply \code{FUN} to each sample (i.e. \code{GatingHierarchy})}
\usage{
\S4method{lapply}{GatingSet}(X, FUN, ...)
}
\arguments{
\item{X}{\code{GatingSet}}

\item{FUN}{\code{function} to be applied to each sample in 'GatingSet'}

\item{...}{other arguments to be passed to 'FUN'}
}
\description{
sample names are used for names of the returned list
}

