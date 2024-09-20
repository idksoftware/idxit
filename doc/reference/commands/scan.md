# Scan

## NAME

idxit add - Add file contents to the index

## SYNOPSIS

```
idxit add [--verbose | -v] [--dry-run | -n] [--force | -f] [--interactive | -i] [--patch | -p]
	  [--edit | -e] [--[no-]all | --[no-]ignore-removal | [--update | -u]] [--sparse]
	  [--intent-to-add | -N] [--refresh] [--ignore-errors] [--ignore-missing] [--renormalize]
	  [--chmod=(+|-)x] [--pathspec-from-file=<file> [--pathspec-file-nul]]
	  [--] [<pathspec>…​]
```

## DESCRIPTION

This command is used the create the index by scanning the contents of the selected storage media following the specification applied in the the options followings the scan comand.

Options.

\--ign=<ignore-file> use this ignore file to ignore contents.

\--nosys Do not use system ignore template. This overrides the default of using the ignore system template.

\--nousys Do not use user system ignore template. This overrides the default of using the ignore user system template.

\--nouser Do not scan the current user folder. This overrides the default of scanning the current user folder.

\--inclass=<class file> include contents of media class file in the scan.

\--exclass=<class-file> exclude contents of media class file in the scan.

\--rootfolder=<path to folder> This will be the starting folder path or root folder form were the scan will begin. This can be used any number times on the command line so can extend the scan scope.

\--ignore=<file> This user defined ignore file.