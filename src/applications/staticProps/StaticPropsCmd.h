/** @file StaticPropsCmd.h
 *  @brief The header file for the command line option parser
 *  generated by GNU Gengetopt version 2.22
 *  http://www.gnu.org/software/gengetopt.
 *  DO NOT modify this file, since it can be overwritten
 *  @author GNU Gengetopt by Lorenzo Bettini */

#ifndef STATICPROPSCMD_H
#define STATICPROPSCMD_H

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h> /* for FILE */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef CMDLINE_PARSER_PACKAGE
/** @brief the program name */
#define CMDLINE_PARSER_PACKAGE "StaticProps"
#endif

#ifndef CMDLINE_PARSER_VERSION
/** @brief the program version */
#define CMDLINE_PARSER_VERSION "1.00"
#endif

/** @brief Where the command line options are stored */
struct gengetopt_args_info
{
  const char *help_help; /**< @brief Print help and exit help description.  */
  const char *version_help; /**< @brief Print version and exit help description.  */
  char * input_arg;	/**< @brief input dump file.  */
  char * input_orig;	/**< @brief input dump file original value given at command line.  */
  const char *input_help; /**< @brief input dump file help description.  */
  char * output_arg;	/**< @brief output file name.  */
  char * output_orig;	/**< @brief output file name original value given at command line.  */
  const char *output_help; /**< @brief output file name help description.  */
  int step_arg;	/**< @brief process every n frame (default='1').  */
  char * step_orig;	/**< @brief process every n frame original value given at command line.  */
  const char *step_help; /**< @brief process every n frame help description.  */
  int nbins_arg;	/**< @brief number of bins (general purpose) (default='100').  */
  char * nbins_orig;	/**< @brief number of bins (general purpose) original value given at command line.  */
  const char *nbins_help; /**< @brief number of bins (general purpose) help description.  */
  int nbins_x_arg;	/**< @brief number of bins in x axis (default='100').  */
  char * nbins_x_orig;	/**< @brief number of bins in x axis original value given at command line.  */
  const char *nbins_x_help; /**< @brief number of bins in x axis help description.  */
  int nbins_y_arg;	/**< @brief number of bins in y axis (default='100').  */
  char * nbins_y_orig;	/**< @brief number of bins in y axis original value given at command line.  */
  const char *nbins_y_help; /**< @brief number of bins in y axis help description.  */
  int nbins_z_arg;	/**< @brief number of bins in z axis (default='100').  */
  char * nbins_z_orig;	/**< @brief number of bins in z axis original value given at command line.  */
  const char *nbins_z_help; /**< @brief number of bins in z axis help description.  */
  int nanglebins_arg;	/**< @brief number of bins for cos(angle) (default='50').  */
  char * nanglebins_orig;	/**< @brief number of bins for cos(angle) original value given at command line.  */
  const char *nanglebins_help; /**< @brief number of bins for cos(angle) help description.  */
  double rcut_arg;	/**< @brief cutoff radius (rcut).  */
  char * rcut_orig;	/**< @brief cutoff radius (rcut) original value given at command line.  */
  const char *rcut_help; /**< @brief cutoff radius (rcut) help description.  */
  double dz_arg;	/**< @brief slab width (dz).  */
  char * dz_orig;	/**< @brief slab width (dz) original value given at command line.  */
  const char *dz_help; /**< @brief slab width (dz) help description.  */
  double length_arg;	/**< @brief maximum length (Defaults to 1/2 smallest length of first frame).  */
  char * length_orig;	/**< @brief maximum length (Defaults to 1/2 smallest length of first frame) original value given at command line.  */
  const char *length_help; /**< @brief maximum length (Defaults to 1/2 smallest length of first frame) help description.  */
  double zlength_arg;	/**< @brief maximum length (Defaults to 1/2 smallest length of first frame).  */
  char * zlength_orig;	/**< @brief maximum length (Defaults to 1/2 smallest length of first frame) original value given at command line.  */
  const char *zlength_help; /**< @brief maximum length (Defaults to 1/2 smallest length of first frame) help description.  */
  double zoffset_arg;	/**< @brief Where to set the zero for the slab_density calculation (default='0').  */
  char * zoffset_orig;	/**< @brief Where to set the zero for the slab_density calculation original value given at command line.  */
  const char *zoffset_help; /**< @brief Where to set the zero for the slab_density calculation help description.  */
  char * sele1_arg;	/**< @brief select the first stuntdouble set.  */
  char * sele1_orig;	/**< @brief select the first stuntdouble set original value given at command line.  */
  const char *sele1_help; /**< @brief select the first stuntdouble set help description.  */
  char * sele2_arg;	/**< @brief select the second stuntdouble set.  */
  char * sele2_orig;	/**< @brief select the second stuntdouble set original value given at command line.  */
  const char *sele2_help; /**< @brief select the second stuntdouble set help description.  */
  char * sele3_arg;	/**< @brief select the third stuntdouble set.  */
  char * sele3_orig;	/**< @brief select the third stuntdouble set original value given at command line.  */
  const char *sele3_help; /**< @brief select the third stuntdouble set help description.  */
  char * refsele_arg;	/**< @brief select reference (use and only use with --gxyz).  */
  char * refsele_orig;	/**< @brief select reference (use and only use with --gxyz) original value given at command line.  */
  const char *refsele_help; /**< @brief select reference (use and only use with --gxyz) help description.  */
  char * comsele_arg;	/**< @brief select stunt doubles for center-of-mass reference point.  */
  char * comsele_orig;	/**< @brief select stunt doubles for center-of-mass reference point original value given at command line.  */
  const char *comsele_help; /**< @brief select stunt doubles for center-of-mass reference point help description.  */
  char * molname_arg;	/**< @brief molecule name.  */
  char * molname_orig;	/**< @brief molecule name original value given at command line.  */
  const char *molname_help; /**< @brief molecule name help description.  */
  int begin_arg;	/**< @brief begin internal index.  */
  char * begin_orig;	/**< @brief begin internal index original value given at command line.  */
  const char *begin_help; /**< @brief begin internal index help description.  */
  int end_arg;	/**< @brief end internal index.  */
  char * end_orig;	/**< @brief end internal index original value given at command line.  */
  const char *end_help; /**< @brief end internal index help description.  */
  double radius_arg;	/**< @brief nanoparticle radius.  */
  char * radius_orig;	/**< @brief nanoparticle radius original value given at command line.  */
  const char *radius_help; /**< @brief nanoparticle radius help description.  */
  const char *bo_help; /**< @brief bond order parameter (--rcut must be specified) help description.  */
  const char *bor_help; /**< @brief bond order parameter as a function of radius (--rcut must be specified) help description.  */
  const char *bad_help; /**< @brief N(theta) bond angle density within (--rcut must be specified) help description.  */
  const char *count_help; /**< @brief count of molecules matching selection criteria (and associated statistics) help description.  */
  const char *gofr_help; /**< @brief g(r) help description.  */
  const char *gofz_help; /**< @brief g(z) help description.  */
  const char *r_theta_help; /**< @brief g(r, cos(theta)) help description.  */
  const char *r_omega_help; /**< @brief g(r, cos(omega)) help description.  */
  const char *r_z_help; /**< @brief g(r, z) help description.  */
  const char *theta_omega_help; /**< @brief g(cos(theta), cos(omega)) help description.  */
  const char *gxyz_help; /**< @brief g(x, y, z) help description.  */
  const char *twodgofr_help; /**< @brief 2D g(r) (Slab width --dz must be specified) help description.  */
  const char *p2_help; /**< @brief p2 order parameter (--sele1 and --sele2 must be specified) help description.  */
  const char *rp2_help; /**< @brief rp2 order parameter (--sele1 and --sele2 must be specified) help description.  */
  const char *scd_help; /**< @brief scd order parameter (either --sele1, --sele2, --sele3 are specified or --molname, --begin, --end are specified) help description.  */
  const char *density_help; /**< @brief density plot help description.  */
  const char *slab_density_help; /**< @brief slab density help description.  */
  const char *p_angle_help; /**< @brief p(cos(theta)) help description.  */
  const char *hxy_help; /**< @brief hxy help description.  */
  const char *rho_r_help; /**< @brief rho of R help description.  */
  const char *hullvol_help; /**< @brief hull volume of nanoparticle help description.  */
  
  unsigned int help_given ;	/**< @brief Whether help was given.  */
  unsigned int version_given ;	/**< @brief Whether version was given.  */
  unsigned int input_given ;	/**< @brief Whether input was given.  */
  unsigned int output_given ;	/**< @brief Whether output was given.  */
  unsigned int step_given ;	/**< @brief Whether step was given.  */
  unsigned int nbins_given ;	/**< @brief Whether nbins was given.  */
  unsigned int nbins_x_given ;	/**< @brief Whether nbins_x was given.  */
  unsigned int nbins_y_given ;	/**< @brief Whether nbins_y was given.  */
  unsigned int nbins_z_given ;	/**< @brief Whether nbins_z was given.  */
  unsigned int nanglebins_given ;	/**< @brief Whether nanglebins was given.  */
  unsigned int rcut_given ;	/**< @brief Whether rcut was given.  */
  unsigned int dz_given ;	/**< @brief Whether dz was given.  */
  unsigned int length_given ;	/**< @brief Whether length was given.  */
  unsigned int zlength_given ;	/**< @brief Whether zlength was given.  */
  unsigned int zoffset_given ;	/**< @brief Whether zoffset was given.  */
  unsigned int sele1_given ;	/**< @brief Whether sele1 was given.  */
  unsigned int sele2_given ;	/**< @brief Whether sele2 was given.  */
  unsigned int sele3_given ;	/**< @brief Whether sele3 was given.  */
  unsigned int refsele_given ;	/**< @brief Whether refsele was given.  */
  unsigned int comsele_given ;	/**< @brief Whether comsele was given.  */
  unsigned int molname_given ;	/**< @brief Whether molname was given.  */
  unsigned int begin_given ;	/**< @brief Whether begin was given.  */
  unsigned int end_given ;	/**< @brief Whether end was given.  */
  unsigned int radius_given ;	/**< @brief Whether radius was given.  */
  unsigned int bo_given ;	/**< @brief Whether bo was given.  */
  unsigned int bor_given ;	/**< @brief Whether bor was given.  */
  unsigned int bad_given ;	/**< @brief Whether bad was given.  */
  unsigned int count_given ;	/**< @brief Whether count was given.  */
  unsigned int gofr_given ;	/**< @brief Whether gofr was given.  */
  unsigned int gofz_given ;	/**< @brief Whether gofz was given.  */
  unsigned int r_theta_given ;	/**< @brief Whether r_theta was given.  */
  unsigned int r_omega_given ;	/**< @brief Whether r_omega was given.  */
  unsigned int r_z_given ;	/**< @brief Whether r_z was given.  */
  unsigned int theta_omega_given ;	/**< @brief Whether theta_omega was given.  */
  unsigned int gxyz_given ;	/**< @brief Whether gxyz was given.  */
  unsigned int twodgofr_given ;	/**< @brief Whether twodgofr was given.  */
  unsigned int p2_given ;	/**< @brief Whether p2 was given.  */
  unsigned int rp2_given ;	/**< @brief Whether rp2 was given.  */
  unsigned int scd_given ;	/**< @brief Whether scd was given.  */
  unsigned int density_given ;	/**< @brief Whether density was given.  */
  unsigned int slab_density_given ;	/**< @brief Whether slab_density was given.  */
  unsigned int p_angle_given ;	/**< @brief Whether p_angle was given.  */
  unsigned int hxy_given ;	/**< @brief Whether hxy was given.  */
  unsigned int rho_r_given ;	/**< @brief Whether rho_r was given.  */
  unsigned int hullvol_given ;	/**< @brief Whether hullvol was given.  */

  char **inputs ; /**< @brief unamed options (options without names) */
  unsigned inputs_num ; /**< @brief unamed options number */
  int staticProps_group_counter; /**< @brief Counter for group staticProps */
} ;

/** @brief The additional parameters to pass to parser functions */
struct cmdline_parser_params
{
  int override; /**< @brief whether to override possibly already present options (default 0) */
  int initialize; /**< @brief whether to initialize the option structure gengetopt_args_info (default 1) */
  int check_required; /**< @brief whether to check that all required options were provided (default 1) */
  int check_ambiguity; /**< @brief whether to check for options already specified in the option structure gengetopt_args_info (default 0) */
  int print_errors; /**< @brief whether getopt_long should print an error message for a bad option (default 1) */
} ;

/** @brief the purpose string of the program */
extern const char *gengetopt_args_info_purpose;
/** @brief the usage string of the program */
extern const char *gengetopt_args_info_usage;
/** @brief all the lines making the help output */
extern const char *gengetopt_args_info_help[];

/**
 * The command line parser
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser (int argc, char * const *argv,
  struct gengetopt_args_info *args_info);

/**
 * The command line parser (version with additional parameters - deprecated)
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @param override whether to override possibly already present options
 * @param initialize whether to initialize the option structure my_args_info
 * @param check_required whether to check that all required options were provided
 * @return 0 if everything went fine, NON 0 if an error took place
 * @deprecated use cmdline_parser_ext() instead
 */
int cmdline_parser2 (int argc, char * const *argv,
  struct gengetopt_args_info *args_info,
  int override, int initialize, int check_required);

/**
 * The command line parser (version with additional parameters)
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @param params additional parameters for the parser
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser_ext (int argc, char * const *argv,
  struct gengetopt_args_info *args_info,
  struct cmdline_parser_params *params);

/**
 * Save the contents of the option struct into an already open FILE stream.
 * @param outfile the stream where to dump options
 * @param args_info the option struct to dump
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser_dump(FILE *outfile,
  struct gengetopt_args_info *args_info);

/**
 * Save the contents of the option struct into a (text) file.
 * This file can be read by the config file parser (if generated by gengetopt)
 * @param filename the file where to save
 * @param args_info the option struct to save
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser_file_save(const char *filename,
  struct gengetopt_args_info *args_info);

/**
 * Print the help
 */
void cmdline_parser_print_help(void);
/**
 * Print the version
 */
void cmdline_parser_print_version(void);

/**
 * Initializes all the fields a cmdline_parser_params structure 
 * to their default values
 * @param params the structure to initialize
 */
void cmdline_parser_params_init(struct cmdline_parser_params *params);

/**
 * Allocates dynamically a cmdline_parser_params structure and initializes
 * all its fields to their default values
 * @return the created and initialized cmdline_parser_params structure
 */
struct cmdline_parser_params *cmdline_parser_params_create(void);

/**
 * Initializes the passed gengetopt_args_info structure's fields
 * (also set default values for options that have a default)
 * @param args_info the structure to initialize
 */
void cmdline_parser_init (struct gengetopt_args_info *args_info);
/**
 * Deallocates the string fields of the gengetopt_args_info structure
 * (but does not deallocate the structure itself)
 * @param args_info the structure to deallocate
 */
void cmdline_parser_free (struct gengetopt_args_info *args_info);

/**
 * Checks that all the required options were specified
 * @param args_info the structure to check
 * @param prog_name the name of the program that will be used to print
 *   possible errors
 * @return
 */
int cmdline_parser_required (struct gengetopt_args_info *args_info,
  const char *prog_name);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* STATICPROPSCMD_H */
