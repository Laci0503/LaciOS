#include"kernel.h"

uint32 memory_pieces[200];
uint8 memory_state[25];
uint8 memory_piece_count;
uint8 mem_get_state(uint8 piece){
    return (memory_state[piece/8] & (0x1 << (piece % 8))) != 0;
}
void mem_set_state(uint8 piece, uint8 state){
    if(state){
        memory_state[piece/8] |= 0x1 << (piece % 8);
    }else{
        memory_state[piece/8] &= ~(0x1 << (piece % 8));
    }
}

void load_memory_map(){
    //print("Loading memory map");
    //nexLine();
    uint64* mmap_address = (uint64*) 1280;
    memroy_map_region_base = (uint64*) 0x6E0;
    memory_map_region_length = (uint64*) 0x6E0+8*10;
    memory_map_region_type = (uint8*)0x6E0+8*10*2;

    uint8 index=0;
    uint64 ba=0;

    for(uint8 i = 0;i<10;i++){
        ba=*mmap_address;
        mmap_address++;
        if(*mmap_address!=0){
            memroy_map_region_base[index]=ba;
            memory_map_region_length[index]=*mmap_address;
            mmap_address++;
            memory_map_region_type[index]=(uint8)(*mmap_address & 0xff);
            mmap_address++;
            index++;
        }else{
            mmap_address+=2;
        }
    }
    memory_map_region_count=index;
}
uint32 avaible_ram;
void print_memory_map(){

    print("Aviable ram: ");
    printint(avaible_ram/(1024*1024));
    print(" MiB");
    nextLine();

    print("Memory map:");

    nextLine();
    goxy(1,getY());
    print("Base address");
    goxy(26,getY());
    print("Length");
    goxy(52,getY());
    print("Type");
    nextLine();
    
    uint8 cl=getY();

    for(uint8 i = 0;i<memory_map_region_count;i++){
        if(memory_map_region_type[i]==1){forecolor=LIGHT_GREEN;}else{forecolor=LIGHT_YELLOW;}
        goxy(1,cl+i);
        print("0x");
        printhex(memroy_map_region_base[i]);
        goxy(26,cl+i);
        print("0x");
        printhex(memory_map_region_length[i]);
        goxy(52,cl+i);
        if(memory_map_region_type[i]==1){
            print("Usable ram");
        }else if(memory_map_region_type[i]==2){
            print("Reserved");
        }else if(memory_map_region_type[i]==3){
            print("ACPI reclaimable memory");
        }else if(memory_map_region_type[i]==4){
            print("ACPI NVS memory");
        }else if(memory_map_region_type[i]==5){
            print("Bad memory");
        }
    }
    forecolor=WHITE;

    printMemoryManagerTable();

}

void printMemoryManagerTable(){
    nextLine();
    print("Memory manager table");
    nextLine();
    print(" Base          State");
    nextLine();
    for(uint8 i=0;i<memory_piece_count;i++){
        print(" 0x");
        printhex(memory_pieces[i]);
        goxy(15,getY());
        if(mem_get_state(i))print("Free"); else print("Reserved");
        nextLine();
    }
}

void free(uint8* address, uint32 length){
    for(uint8 i=0;i<memory_piece_count;i++){
        if(memory_pieces[i]>=(uint32)address || i==memory_piece_count-1){
            if(memory_pieces[i] == address){
                mem_set_state(i,1);
                for(uint8 j=i;j<memory_piece_count-2;j++){
                    memory_pieces[j]=memory_pieces[j+2];
                    mem_set_state(j,mem_get_state(j+2));
                }
                memory_piece_count-=2;
            }else{
                for(uint8 j=memory_piece_count;j>=i;j--){
                    memory_pieces[j]=memory_pieces[j-1];
                    mem_set_state(j,mem_get_state(j-1));
                }
                memory_piece_count++;
                memory_pieces[i]=address;
                mem_set_state(i,1);
                if(memory_pieces[i+1] != address+length){
                    for(uint8 j=memory_piece_count;j>i;j--){
                        memory_pieces[j]=memory_pieces[j-1];
                        mem_set_state(j,mem_get_state(j-1));
                    }
                    memory_piece_count++;
                    memory_pieces[i+1]=address+length;
                    mem_set_state(i+1,0);
                }else{
                    for(uint8 j=i+1;j<memory_piece_count-1;j++){
                        memory_pieces[j]=memory_pieces[j+1];
                        mem_set_state(j,mem_get_state(j+1));
                    }
                    memory_piece_count--;
                }
            }
            break;
        }
    }
}

uint8* malloc(uint32 length){
    for(uint8 i=0;i<memory_piece_count;i++){
        if(mem_get_state(i) && memory_pieces[i+1] - memory_pieces[i]>=length){
            for(uint8 j=memory_piece_count;j>i+1;j--){
                memory_pieces[j]=memory_pieces[j-1];
                mem_set_state(j,mem_get_state(j-1));
            }

            uint8* return_val = (uint8*)memory_pieces[i];

            memory_piece_count+=1;
            mem_set_state(i,0);
            memory_pieces[i+1]=memory_pieces[i]+length;
            mem_set_state(i+1,1);

            for(uint8 j=i+1;j<memory_piece_count;j++){
                memory_pieces[j-1]=memory_pieces[j];
                mem_set_state(j-1,mem_get_state(j));
            }
            memory_piece_count--;

            return return_val;
        }
    }
}

void mem_clear_overleaps(){
    uint8 q=1;
    while(q){
        q=0;
        for(uint8 i=0;i<memory_piece_count-1;i++){
            if(memory_pieces[i]==memory_pieces[i+1]){
                print("address ");
                memory_pieces[i]=memory_pieces[i+1];
                mem_set_state(i,mem_get_state(i+1));
                for(uint8 j=i+1;j<memory_piece_count;j++){
                    memory_pieces[j]=memory_pieces[j+1];
                    mem_set_state(j,mem_get_state(j+1));
                }
                memory_piece_count--;
                q=1;
                break;

            }
            if(mem_get_state(i)==mem_get_state(i+1)){
                print("state ");
                for(uint8 j=i;j<memory_piece_count;j++){
                    memory_pieces[j]=memory_pieces[j+1];
                    mem_set_state(j,mem_get_state(j+1));
                }
                memory_piece_count--;
                q=1;
                break;
            }
        }
    }
}

void memory_init(){
    uint8 q=1;
    uint8 t=0;
    while(q){
        q=0;
        for(uint8 i=0;i<memory_map_region_count-1;i++){
            if((uint32)memroy_map_region_base[i]>(uint32)memroy_map_region_base[i+1]){
                uint32 c = memroy_map_region_base[i];
                memroy_map_region_base[i]=memroy_map_region_base[i+1];
                memroy_map_region_base[i+1]=c;

                c=memory_map_region_length[i];
                memory_map_region_length[i]=memory_map_region_length[i+1];
                memory_map_region_length[i+1]=c;

                c=memory_map_region_type[i];
                memory_map_region_type[i]=memory_map_region_type[i+1];
                memory_map_region_type[i+1]=c;

                q=1;
            }
        }
        t++;
    }

    avaible_ram=0;

    //uint32 memory_start=(*(uint16*)0x8c3+1) * 1024;

    uint32 mem_length=0;

    for(uint8 i=memory_map_region_count;i>0;i--){
        if(memroy_map_region_base[i-1]==0x100000){
            avaible_ram+=memory_map_region_length[i-1];
            mem_length=memory_map_region_length[i-1];
        }
    }

    memory_pieces[0]=0;
    mem_set_state(0,0);
    memory_pieces[1]=0x100000;
    mem_set_state(1,1);
    memory_pieces[2]=0x100000 + mem_length;
    mem_set_state(2,0);
    memory_piece_count=3;
}