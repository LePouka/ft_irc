#pragma once

/******************************************************************************/
/*   INCLUDES                                                                 */
/******************************************************************************/
# include <iostream>
# include <string>

/******************************************************************************/
/*   DEFINES                                                                  */
/******************************************************************************/

/******************************************************************************/
/*   STRUCTURS                                                                */
/******************************************************************************/
struct Channel {

	std::string		name;
	std::string		topic;
	bool			isInviteOnly;
	bool			isTopicRestrictedToOperators;
	unsigned int	userLimit;

};

/******************************************************************************/
/*   CLASSES                                                                  */
/******************************************************************************/

class ChannelManager {

private:

protected:

public:

	// Constructors
	Channel( void );
	Channel( Channel const & src );

	// Destructor
	virtual ~Channel( void );

	// Operators
	Channel&	operator=( Channel const & rhs );

};

/******************************************************************************/
/*   REDIRECTION OPERATOR                                                     */
/******************************************************************************/
std::ostream&	operator<<( std::ostream& o, Channel const & rhs );
