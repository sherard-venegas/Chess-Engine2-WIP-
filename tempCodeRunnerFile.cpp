
    
    dave.set_boards( jim.get_BK(), jim.get_BQ(), jim.get_BR(), jim.get_BB(), jim.get_BN(), jim.get_BP(),
                     jim.get_WK(), jim.get_WQ(), jim.get_WR(), jim.get_WB(), jim.get_WN(), jim.get_WP());

    dave.print_bitb(6);
    cout << "\n";
    dave.print_board( dave.king_attacks(0) );
    dave.print_mask_funcQ(27);
    return 0;
}
