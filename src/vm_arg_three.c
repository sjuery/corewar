/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm_arg_three.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlu <mlu@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/17 20:59:44 by mlu               #+#    #+#             */
/*   Updated: 2018/02/23 17:37:33 by anazar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void	vm_lld(t_vm *vm, int i)
{
	(void)vm;
	(void)i;
	ft_printf("lld called");
}


void	vm_lldi(t_vm *vm, int i)
{
	(void)vm;
	(void)i;
	ft_printf("lldi called");
}

void	vm_ldi(t_vm *vm, int i)
{
	(void)vm;
	(void)i;
	ft_printf("ldi called");
}


void	vm_ld(t_vm *vm, int i)
{
	read_acb(vm->info[i].body[vm->info[i].index + 1]);
	ft_printf("ld called %i", vm->info[i].body[vm->info[i].index + 1]);
}

void	vm_st(t_vm *vm, int i)
{
	(void)vm;
	(void)i;
	ft_printf("st called");
}
