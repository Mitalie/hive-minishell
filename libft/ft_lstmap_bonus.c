/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 10:25:12 by amakinen          #+#    #+#             */
/*   Updated: 2024/04/26 17:49:44 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	**prev_next;
	t_list	*new_curr;

	new_lst = 0;
	prev_next = &new_lst;
	while (lst)
	{
		new_curr = malloc(sizeof(t_list));
		if (!new_curr)
		{
			ft_lstclear(&new_lst, del);
			return (0);
		}
		*prev_next = new_curr;
		new_curr->content = f(lst->content);
		new_curr->next = 0;
		prev_next = &new_curr->next;
		lst = lst->next;
	}
	return (new_lst);
}
